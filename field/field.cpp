#include "field.h"
#include "cell.h"
#include "cellinformation.h"
#include "cellrule.h"
#include "fieldinformation.h"
#include "scene/sceneItem.h"
#include "properties.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QPainter>

Field::Field(int width, int height, QObject *parent)
    : QObject(parent),
    m_Width(width),
    m_Height(height),
    m_Rule(nullptr),
    m_Running(false),
    m_RunningAlways(false),
    m_WaitScene(false),
    m_StopCalculating(false)
{
    setObjectName(QString("FIELD[%1X%2]").arg(QString::number(width), QString::number(height)));

    m_FieldInformation = new FieldInformation(this);
    m_Cells = QVector(m_Width, QVector<Cell*>(m_Height, nullptr));

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void Field::fill()
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    for(int h = 0; h < m_Height; h++)
    {
        for(int w = 0; w < m_Width; w++)
        {
            addCell(w, h);
        }
        Q_EMIT signalFillingProgress(h + 1);
    }
    qDebug() << "Field" << objectName() << "filled in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

Cell *Field::addCell(int x, int y)
{
    auto c = new Cell;
    c->moveToThread(thread()); c->setParent(this); // NOTE: field выполняется не в основном потоке
    c->setField(this);
    c->setIndex({x, y});
    c->setObjectName(QString("[%1X%2]").arg(QString::number(x), QString::number(y)));
    m_Cells[x][y] = c;
    return c;
}

Cell *Field::addCell(QPoint index) { return addCell(index.x(), index.y()); }

Cell *Field::getTopCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getTopLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getTopRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

QVector<Cell*> Field::getCellsAround(Cell *c)
{
    QVector<Cell*> result;

    result.append(getTopCell(c));
    result.append(getTopRightCell(c));
    result.append(getRightCell(c));
    result.append(getBottomRightCell(c));
    result.append(getBottomCell(c));
    result.append(getBottomLeftCell(c));
    result.append(getLeftCell(c));
    result.append(getTopLeftCell(c));

    return result;
}

void Field::setRunningAlways(bool value)
{
    if(m_RunningAlways == value) return;

    m_RunningAlways = value;
}

void Field::setRule(CellRule *value)
{
    if(!value) return;
    if(m_Rule == value) return;

    if(m_Rule) m_Rule->deleteLater();

    m_Rule = value;

    m_Rule->moveToThread(thread());
    m_Rule->setParent(this);

    Q_EMIT signalRuleChanged(m_Rule);
}

void Field::setRunning(bool value)
{
    if(m_Running == value) return;

    m_Running = value;
    Q_EMIT signalRunning(m_Running);
}

void Field::calculate()
{
    qDebug() << "Field calculating started";
    while(m_Running)
    {
        if(m_StopCalculating) break;
        if(m_WaitScene) continue;

        auto time = QDateTime::currentMSecsSinceEpoch();

        m_FieldInformation->stepAge();

        auto pixmap = createPixmap();
        QPainter painter(&pixmap);

        for(int h = 0; h < m_Height; h++)
        {
            if(m_StopCalculating) break;
            for(int w = 0; w < m_Width; w++)
            {
                if(m_StopCalculating) break;

                auto c = m_Cells.at(w).at(h);

                // TODO: выполнение правил
                auto state = c->getInformation()->getState();
                if(state == Kernel::CellState::Dead)
                {
                    c->getInformation()->setState(Kernel::CellState::Alive);
                }
                else if(state == Kernel::CellState::Alive)
                {
                    c->getInformation()->setState(Kernel::CellState::Cursed);
                }
                else if(state == Kernel::CellState::Cursed)
                {
                    c->getInformation()->setState(Kernel::CellState::Dead);
                }

                drawCell(c, &painter);
            }
        }

        if(!m_RunningAlways) m_Running = false;
        m_FieldInformation->applyAverageCalc(time);
        m_WaitScene = true;
        Q_EMIT signalCalculated(QVariant(pixmap));

        // пауза
        auto pausetime = QDateTime::currentDateTime().addMSecs(config->SceneCalculatingMinPause());
        while(QDateTime::currentDateTime() < pausetime && !m_StopCalculating)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    qDebug() << "Field calculating stopped";
    Q_EMIT signalCalculatingStopped();
}

QPixmap Field::createPixmap()
{
    auto pixmap = QPixmap(m_Width * config->SceneCellSize(),
                          m_Height * config->SceneCellSize());

    pixmap.fill(config->SceneCellDeadColor());
    return pixmap;
}

void Field::drawCell(Cell* cell, QPainter *painter)
{
    QRect rect(cell->getIndex().x() * config->SceneCellSize(),
               cell->getIndex().y() * config->SceneCellSize(),
               config->SceneCellSize(),
               config->SceneCellSize());

    auto state = cell->getInformation()->getState();

    if(state == Kernel::CellState::Dead)
    {
        // ничего не делаем
    }
    else if(state == Kernel::CellState::Alive)
    {
        painter->fillRect(rect, m_Rule->getColorAlive());
    }
    else if(state == Kernel::CellState::Cursed)
    {
        painter->fillRect(rect, config->SceneCellCurseColor());
    }
}

CellRule *Field::getRule() const { return m_Rule; }
Cell *Field::getCell(QPoint index) { return m_Cells[index.x()][index.y()]; }
QVector<QVector<Cell *>> *Field::cells() const { return const_cast<QVector<QVector<Cell*>>*>(&m_Cells); }
int Field::height() { return m_Height; }
int Field::width() { return m_Width; }
bool Field::getRunningAlways() const { return m_RunningAlways; }
bool Field::isRunning() { return m_Running; }
FieldInformation *Field::getInformation() const { return m_FieldInformation; }
void Field::setWaitScene(bool value) { m_WaitScene = value; }
void Field::StopCalculating() { m_StopCalculating = true; }
bool Field::getWaitScene() const { return m_WaitScene; }
