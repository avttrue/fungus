#include "scene.h"
#include "properties.h"
#include "sceneItem.h"
#include "sceneview.h"
#include "helper.h"
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
      m_SceneItem(nullptr),
      m_SelectionMark(nullptr),
      m_AverageDraw(0),
      m_StopAdvanse(false)
{
    m_Size = QSize(m_Field->width() * config->SceneCellSize(), m_Field->height()) * config->SceneCellSize();
    setObjectName(QString("SCENE[%1X%2]").
                  arg(QString::number(m_Field->width()), QString::number(m_Field->height())));

    if(config->SceneBspTreeIndex())
    {
        setItemIndexMethod(QGraphicsScene::BspTreeIndex);
        setBspTreeDepth(config->SceneBspTreeDepth());
    }
    else setItemIndexMethod(QGraphicsScene::NoIndex);

    if(QColor::isValidColor(config->SceneBgColor()))
        setBackgroundColor(QColor(config->SceneBgColor()));
    else setBackgroundColor(QColor(SCENE_BG_COLOR));

    addSceneItem();
    addSelectionMark();

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
        qCritical() << "SceneItem not created";
        return;
    }

    QPen pen(QColor(config->SceneSelectColor()));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    QBrush brush(QColor(config->SceneSelectColor()));

    m_SelectionMark = addRect(0, 0, config->SceneCellSize(), config->SceneCellSize(), pen, brush);
    m_SelectionMark->setZValue(m_SceneItem->zValue() + 1);
    m_SelectionMark->hide();
}

void Scene::selectCell(Cell *cell)
{
    if(m_SelectedCell == cell) return;

    if(!cell)
    {
        m_SelectionMark->hide();
    }
    else
    {
        m_SelectionMark->setPos(m_SceneItem->mapToParent(cell->getIndex() * config->SceneCellSize()));
        m_SelectionMark->show();
    }

    if(!cell) qDebug() << "Cell unselected:" << m_SelectedCell->objectName();
    else qDebug() << "Cell selected:" << cell->objectName();

    m_SelectedCell = cell;

    Q_EMIT signalSelectedCellChanged(m_SelectedCell);
}

void Scene::setBackgroundColor(const QColor &value)
{
    m_BackgroundColor = value;
    setBackgroundBrush(m_BackgroundColor);
}

void Scene::slotAdvance(QVector<Cell *> cells)
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    auto pixmap = m_SceneItem->getPixmap();

    pixmap->fill(config->SceneCellDeadColor());

    QPainter painter;
    painter.begin(pixmap);

    for(auto c: cells)
    {
        if(m_StopAdvanse) break;

        QRect rect(c->getIndex().x() * config->SceneCellSize(),
                   c->getIndex().y() * config->SceneCellSize(),
                   config->SceneCellSize(),
                   config->SceneCellSize());

        auto state = c->getCurInfo()->getState();
        // Kernel::CellState::Dead не обрабатываем
        if(state == Kernel::CellState::Alive)
            painter.fillRect(rect, m_Field->getRule()->getColorAlive());
        else if(state == Kernel::CellState::Cursed)
            painter.fillRect(rect, config->SceneCellCurseColor());
    }

    painter.end();

    if(m_StopAdvanse) return;

    applyAverageDraw(time);

    advance();
    Q_EMIT signalReady();
}

void Scene::applyAverageDraw(qint64 time)
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

Cell *Scene::getSelectedCell() const { return m_SelectedCell; }
QSize Scene::size() const { return m_Size; }
QColor Scene::getBackgroundColor() const { return m_BackgroundColor; }
Field *Scene::getField() const { return m_Field; }
void Scene::StopAdvanse() { m_StopAdvanse = true; }
SceneItem *Scene::getSceneItem() const { return m_SceneItem; }
qreal Scene::getAverageDraw() const { return m_AverageDraw; }


