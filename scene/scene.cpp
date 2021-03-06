#include "scene.h"
#include "properties.h"
#include "sceneitem.h"
#include "sceneview.h"
#include "helpers/helper.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/fieldrule.h"
#include "field/cellinformation.h"
#include "field/fieldinformation.h"

#include <QDateTime>
#include <QDebug>

Scene::Scene(QObject* parent, Field *field)
    : QGraphicsScene(parent),
      m_Field(field),
      m_SelectedCell(nullptr),
      m_SecondSelectedCell(nullptr),
      m_SceneItem(nullptr),
      m_SelectionMark(nullptr),
      m_MultiSelectionMark(nullptr),
      m_AverageDraw(0),
      m_StopAdvanse(false)
{
    setObjectName(QString("SCENE[%1X%2]").
                  arg(QString::number(m_Field->width()), QString::number(m_Field->height())));

    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundColor(QColor(config->SceneBgColor()));

    addSceneItem();
    addSelectionMark();
    addMultiSelectionMark();

    applyCellIndication();
    applyCellsColors();

    m_Size = m_SceneItem->rect().size();

    QObject::connect(m_Field, &Field::signalCalculated, this, &Scene::slotAdvance, Qt::DirectConnection);
    QObject::connect(this, &Scene::signalReady, m_Field, &Field::slotSceneReady, Qt::DirectConnection);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void Scene::addSceneItem()
{
    m_SceneItem = new SceneItem(this);
    m_SceneItem->setPos(m_SceneItem->mapToParent(0, 0));
    m_SceneItem->setZValue(config->SceneItemZValue());
    addItem(m_SceneItem);
}

void Scene::addSelectionMark()
{
    if(!m_SceneItem)
    {
        qCritical() << __func__ << "SceneItem not created";
        return;
    }

    QPen pen;
    QBrush brush;
    QRect rect(0, 0, config->SceneCellSize(), config->SceneCellSize());
    m_SelectionMark = addRect(rect, pen, brush);
    m_SelectionMark->setZValue(m_SceneItem->zValue() + 1);
    setSelectionMarkColor(config->SceneSelectColor());

    m_SelectionMark->hide();
}

void Scene::addMultiSelectionMark()
{
    QPen pen;
    QBrush brush;
    QRect rect(0, 0, 0, 0);
    m_MultiSelectionMark = addRect(rect, pen, brush);
    m_MultiSelectionMark->setZValue(m_SceneItem->zValue() + 2);
    setMultiSelectionMarkColor(config->SceneSelectColor());

    m_MultiSelectionMark->hide();
}

void Scene::setSelectionMarkColor(const QString &color)
{
    if(!m_SelectionMark) return;

    auto pen = QPen(QColor(color));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    auto brushColor = QColor(color);
    brushColor.setAlpha(config->SceneSelAlfa());

    m_SelectionMark->setPen(pen);
    m_SelectionMark->setBrush(brushColor);
}

void Scene::setMultiSelectionMarkColor(const QString &color)
{
    if(!m_MultiSelectionMark) return;

    auto pen = QPen(QColor(color));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    auto brushColor = QColor(color);
    brushColor.setAlpha(config->SceneMultiselAlfa());
    auto brush = QBrush(brushColor);

    m_MultiSelectionMark->setPen(pen);
    m_MultiSelectionMark->setBrush(brush);
}

void Scene::applyCellIndication()
{
    c_alive_ind.on = config->CellAliveAgeIndicate();
    c_alive_ind.lighter = config->CellAliveAgeIndicBright() == SCENE_CELL_BRIGHTNESS_VALUES.at(0);
    c_alive_ind.diapason = config->CellAliveAgeIndicDiapason();
    c_alive_ind.factor = config->CellAliveAgeIndicFactor();
    c_alive_ind.factor_step = (c_alive_ind.factor - 100) / c_alive_ind.diapason;

    c_curse_ind.on = config->CellCurseAgeIndicate();
    c_curse_ind.lighter = config->CellCurseAgeIndicBright() == SCENE_CELL_BRIGHTNESS_VALUES.at(0);
    c_curse_ind.diapason = config->CellCurseAgeIndicDiapason();
    c_curse_ind.factor = config->CellCurseAgeIndicFactor();
    c_curse_ind.factor_step = (c_curse_ind.factor - 100) / c_curse_ind.diapason;
}

void Scene::applyCellsColors()
{
    auto alive_color = config->SceneCellAliveColor();
    if(config->CellAliveAgeIndicate() && alive_color == "#000000")
        alive_color = "#111111"; // для цвета #000000 не работает QColor::lighter()
    m_AliveCellColor = QColor(alive_color);

    auto trait_color = config->SceneCellTraitColor();
    m_TraitCellColor = QColor(trait_color);

    auto curse_color = config->SceneCellCurseColor();
    if(config->CellCurseAgeIndicate() && curse_color == "#000000")
        curse_color = "#111111"; // для цвета #000000 не работает QColor::lighter()
    m_CurseCellColor = QColor(curse_color);

    m_DeadCellColor =  QColor(config->SceneCellDeadColor());
}

void Scene::selectCell(Cell *cell, bool showinfo)
{
    if(cell && m_SelectedCell == cell)
    {
        if (showinfo) Q_EMIT signalShowCellInfo();
        return;
    }

    MultiselectCell(nullptr);

    if(!cell)
    {
        m_SelectionMark->hide();
        if(m_SelectedCell) qDebug() << "Cell unselected:" << m_SelectedCell->objectName();
    }
    else
    {
        m_SelectionMark->setPos(cell->getRect().topLeft());
        m_SelectionMark->show();
        qDebug() << "Cell selected:" << cell->objectName();
    }

    m_SelectedCell = cell;
    update(); // NOTE: ? костыль для ситуации остановки обновления SceneItem

    Q_EMIT signalSelectedCellChanged(m_SelectedCell);
}

void Scene::MultiselectCell(Cell *cell)
{
    if(!cell)
    {
        clearMultiSelection();
        if(m_SecondSelectedCell) qDebug() << "Second cell unselected:" << m_SecondSelectedCell->objectName();
    }
    else if(m_SelectedCell && m_SelectedCell != cell)
    {
        m_SecondSelectedCell = cell;
        auto rect = m_SelectedCell->getRect().united(cell->getRect());
        m_MultiSelectionMark->setRect(rect);
        m_MultiSelectionMark->show();
        qDebug() << "Second cell selected:" << cell->objectName();
    }
    update(); // NOTE: ? костыль для ситуации остановки обновления SceneItem
    Q_EMIT signalSelectedCellsChanged(m_SelectedCell, m_SecondSelectedCell);
}

void Scene::setBackgroundColor(const QColor &value)
{
    m_BackgroundColor = value;
    setBackgroundBrush(m_BackgroundColor);
}

void Scene::slotAdvance(QVector<Cell *> cells)
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    auto pixmap = m_SceneItem->getBuffer();

    QPainter painter;
    painter.begin(pixmap);
    pixmap->fill(m_DeadCellColor);

    for(auto c: cells)
    {
        if(m_StopAdvanse) break;

        auto ci = c->getNewInfo();
        switch (ci->getState())
        {
        case Kernel::CellState::ALIVE:
        {
            auto color = ci->isTrait() ? m_TraitCellColor : m_AliveCellColor;
            // индикация возраста живой ячейки без особенности
            if(c_alive_ind.on && !ci->isTrait())
            {
                auto factor = c_alive_ind.factor;
                if(ci->getAge() < c_alive_ind.diapason)
                    factor = 100 + ci->getAge() * c_alive_ind.factor_step;

                if(c_alive_ind.lighter) color = color.lighter(factor);
                else color = color.darker(factor);
            }
            painter.fillRect(c->getRect(), color);
            break;
        }
        case  Kernel::CellState::CURSED:
        {
            auto color = m_CurseCellColor;
            if(c_curse_ind.on) // индикация возраста отравленной ячейки
            {
                auto factor = c_curse_ind.factor;
                if(ci->getAge() < c_curse_ind.diapason)
                    factor = 100 + ci->getAge() * c_curse_ind.factor_step;

                if(c_curse_ind.lighter) color = color.lighter(factor);
                else color = color.darker(factor);
            }
            painter.fillRect(c->getRect(), color);
            break;
        }
        default: break;
        }
    }
    painter.end();

    if(m_StopAdvanse) return;

    advance();
    applyAverageDraw(time);

    Q_EMIT signalReady();
}

void Scene::applyAverageDraw(uint time)
{
    auto new_d = QDateTime::currentMSecsSinceEpoch() - time;

    qreal new_ad = calcAverage(m_AverageDraw, m_Field->getInformation()->getAge(), new_d);

    if (m_AverageDraw > new_ad || m_AverageDraw < new_ad)
    {
        auto up = false;
        if(m_AverageDraw < new_ad) up = true;
        m_AverageDraw = new_ad;

        if(up) Q_EMIT signalAverageDrawChangedUp(new_ad);
        else Q_EMIT signalAverageDrawChangedDown(new_ad);
    }
}

void Scene::clearMultiSelection()
{
    m_SecondSelectedCell = nullptr;
    m_MultiSelectionMark->hide();
    Q_EMIT signalSelectedCellsChanged(m_SelectedCell, m_SecondSelectedCell);
}

Cell *Scene::getSecondSelectedCell() const { return m_SecondSelectedCell; }
Cell *Scene::getSelectedCell() const { return m_SelectedCell; }
QSize Scene::size() const { return m_Size; }
QColor Scene::getBackgroundColor() const { return m_BackgroundColor; }
Field *Scene::getField() const { return m_Field; }
void Scene::StopAdvanse() { m_StopAdvanse = true; }
SceneItem *Scene::getSceneItem() const { return m_SceneItem; }
qreal Scene::getAverageDraw() const { return m_AverageDraw; }


