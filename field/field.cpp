#include "field.h"
#include "cell.h"
#include "cellinformation.h"
#include "fieldrule.h"
#include "fieldinformation.h"
#include "properties.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>

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
    m_CellsCount = width * height;

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
    m_FieldInformation->upAge();
    m_FieldInformation->setDeadCells(m_Width * m_Height);
    m_FieldInformation->setAliveCells(0);
    m_FieldInformation->setCursedCells(0);

    qDebug() << "Field" << objectName() << "filled in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

void Field::calculate()
{
    qDebug() << "Field calculating started";
    while(m_Running)
    {
        if(m_WaitScene) continue;

        auto time = QDateTime::currentMSecsSinceEpoch();

        m_FieldInformation->upAge();
        qint64 alive = 0;
        qint64 cursed = 0;

        QVector<Cell*> cells;
        for(int h = 0; h < m_Height; h++)
        {
            if(m_StopCalculating) break;
            for(int w = 0; w < m_Width; w++)
            {
                if(m_StopCalculating) break;

                auto c = m_Cells.at(w).at(h);
                auto ci = c->getCurInfo();
                auto ni = c->getNewInfo();

                if(m_RuleOn)
                {
                    //testRules(c); // test
                    applyRules(c);

                    // итог применения правила к ячейке
                    // cell Age
                    if(ci->getState() == Kernel::CellState::Alive) ni->upAge();
                    else ni->setAge(0);

                    // cell Generation
                    if(ci->getAge() == 0 && ni->getAge() > 0) ni->upGeneration();
                }

                // Field Information
                auto nis = ni->getState();
                //if(nis == Kernel::CellState::Dead) // не передаём
                if(nis == Kernel::CellState::Alive)
                {
                    cells.append(c);
                    alive++;
                }
                else if(nis == Kernel::CellState::Cursed)
                {
                    cells.append(c);
                    cursed++;
                }
            }
        }
        if(m_StopCalculating) break;

        if(m_RuleOn) applyCalculating();

        m_FieldInformation->setDeadCells(m_CellsCount - alive - cursed);
        m_FieldInformation->setAliveCells(alive);
        m_FieldInformation->setCursedCells(cursed);
        m_FieldInformation->applyAverageCalc(time);

        m_WaitScene = true;
        Q_EMIT signalCalculated(cells);

        if(!m_RunningAlways) setRunning(false);

        // пауза
        if(m_RunningAlways && m_RuleOn)
        {
            auto pausetime = QDateTime::currentDateTime().addMSecs(config->SceneCalculatingMinPause());
            while(QDateTime::currentDateTime() < pausetime && !m_StopCalculating)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
    }

    qDebug() << "Field calculating stopped";
    Q_EMIT signalCalculatingStopped();
}

void Field::applyCalculating()
{
    for(int h = 0; h < m_Height; h++)
    {
        if(m_StopCalculating) break;
        for(int w = 0; w < m_Width; w++)
        {
            if(m_StopCalculating) break;

            auto c = m_Cells.at(w).at(h);
            c->applyNewInfo();
        }
    }
}

Cell *Field::addCell(int x, int y)
{
    auto c = new Cell(this);
    c->moveToThread(thread()); c->setParent(this); // NOTE: field выполняется не в основном потоке
    c->setIndex({x, y});
    c->setObjectName(QString("[%1X%2]").arg(QString::number(x), QString::number(y)));
    m_Cells[x][y] = c;
    return c;
}

Cell *Field::addCell(QPoint index) { return addCell(index.x(), index.y()); }

Cell *Field::getTopCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getRightCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getBottomCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getLeftCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getTopLeftCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getTopRightCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getBottomLeftCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

Cell *Field::getBottomRightCell(Cell *cell)
{
    auto index = cell->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto c = getCell(index);
    if(!c) c = addCell(index);

    return c;
}

QVector<Cell*> Field::getCellsAround(Cell *cell)
{
    QVector<Cell*> result;

    result.append(getTopCell(cell));
    result.append(getTopRightCell(cell));
    result.append(getRightCell(cell));
    result.append(getBottomRightCell(cell));
    result.append(getBottomCell(cell));
    result.append(getBottomLeftCell(cell));
    result.append(getLeftCell(cell));
    result.append(getTopLeftCell(cell));

    return result;
}

QVector<Cell *> Field::getCellsAroundByStatus(Cell *cell, Kernel::CellState status)
{
    QVector<Cell*> result;

    auto nc = getTopCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getTopRightCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getRightCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getBottomRightCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getBottomCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getBottomLeftCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getLeftCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    nc = getTopLeftCell(cell);
    if(nc->getCurInfo()->getState() == status) result.append(nc);

    return result;
}

void Field::setRunningAlways(bool value)
{
    if(m_RunningAlways == value) return;

    m_RunningAlways = value;
}

void Field::setRule(FieldRule *value)
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

void Field::testRules(Cell *c)
{
    auto ci = c->getCurInfo();
    auto ni = c->getNewInfo();


    if(ci->getState() == Kernel::CellState::Dead)
    {
        ni->setState(Kernel::CellState::Alive);
    }
    else if(ci->getState() == Kernel::CellState::Alive)
    {
        ni->setState(Kernel::CellState::Cursed);
    }
    else if(ci->getState() == Kernel::CellState::Cursed)
    {
        ni->setState(Kernel::CellState::Dead);
    }
}

void Field::applyRules(Cell *cell) // TODO: выполнение правил
{
    auto ci = cell->getCurInfo();
    auto ni = cell->getNewInfo();

    // {ActivityType, SelfState, ActivityTarget, TargetState, ActivityOperand, ActivityOperator, [значение]};
    for(auto a: m_Rule->getActivity())
    {
        if(a.count() < 7)
        {
            qCritical() << "Incorrect activity:" << a;
            continue;
        }

        auto sstate = static_cast<Kernel::CellState>(a.value(1).toInt());   // self state
        if(ci->getState() != sstate) continue;

        auto atype = static_cast<Kernel::ActivityType>(a.value(0).toInt());
        auto atarget = static_cast<Kernel::ActivityTarget>(a.value(2).toInt());
        auto tstate = static_cast<Kernel::CellState>(a.value(3).toInt());
        auto aoperand = static_cast<Kernel::ActivityOperand>(a.value(4).toInt());
        auto aoperator = static_cast<Kernel::ActivityOperator>(a.value(5).toInt());
        auto value =  a.value(6).toInt();

        auto list = getCellsAroundByStatus(cell, tstate);

        // значение в операнде
        auto operand_value = [aoperand, list]()
        {
            auto count = 0;
            if(aoperand == Kernel::ActivityOperand::Count)
            { count = list.count(); }

            else if(aoperand == Kernel::ActivityOperand::Age)
            { for(Cell* c: list) count += c->getCurInfo()->getAge(); }

            return count;
        };

        // учёт ActivityType
        auto activity_reaction = [atype, ni, list]()
        {
            if(atype == Kernel::ActivityType::Birth)
            { ni->setState(Kernel::CellState::Alive);}

            else if(atype == Kernel::ActivityType::Death)
            { ni->setState(Kernel::CellState::Dead); }

            else if(atype == Kernel::ActivityType::Bomb)
            { for(Cell* c: list) c->getCurInfo()->setState(Kernel::CellState::Cursed); }
        };

        // применение оператора к операнду
        switch(aoperator)
        {
        case Kernel::ActivityOperator::Equal:

            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getCurInfo()->getAge();
                if(count == value) activity_reaction();
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = operand_value();
                if(count == value) activity_reaction();
            }
            break;

        case Kernel::ActivityOperator::Less:

            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getCurInfo()->getAge();
                if(count < value) activity_reaction();
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = operand_value();
                if(count < value) activity_reaction();
            }
            break;

        case Kernel::ActivityOperator::More:

            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getCurInfo()->getAge();
                if(count > value) activity_reaction();
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = operand_value();
                if(count > value) activity_reaction();
            }
            break;

        default: break;
        }
    }
}

FieldRule *Field::getRule() const { return m_Rule; }
Cell *Field::getCell(QPoint index) { return m_Cells[index.x()][index.y()]; }
QVector<QVector<Cell *>> *Field::cells() const { return const_cast<QVector<QVector<Cell*>>*>(&m_Cells); }
int Field::height() { return m_Height; }
int Field::width() { return m_Width; }
bool Field::isRunning() { return m_Running; }
FieldInformation *Field::getInformation() const { return m_FieldInformation; }
void Field::slotSceneReady() { m_WaitScene = false; }
void Field::StopCalculating() { m_StopCalculating = true; }
void Field::setRuleOn(bool value) { m_RuleOn = value; }
qint64 Field::getCellsCount() const { return m_CellsCount; }
