#include "field.h"
#include "cell.h"
#include "cellinformation.h"
#include "fieldrule.h"
#include "fieldinformation.h"
#include "properties.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

Field::Field(int width, int height, QObject *parent)
    : QObject(parent),
      m_Width(width),
      m_Height(height),
      m_Rule(nullptr),
      m_Calculating(false),
      m_CalculatingNonstop(false),
      m_WaitScene(false),
      m_AbortCalculating(false)
{
    setObjectName(QString("FIELD[%1X%2]").arg(QString::number(width), QString::number(height)));

    m_FieldInformation = new FieldInformation(this);
    m_FieldInformation->setCellsCount(width * height);

    m_Cells = QVector(m_Width, QVector<Cell*>(m_Height, nullptr));

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

Cell *Field::addCell(QPoint index) { return addCell(index.x(), index.y()); }

void Field::fill(bool random)
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    uint alive = 0;
    uint cursed = 0;
    auto rg = QRandomGenerator::securelySeeded();

    for(int h = 0; h < m_Height; h++)
    {
        for(int w = 0; w < m_Width; w++)
        {
            auto c = addCell(w, h);
            if(random)
            {
                if(rg.bounded(0, 2))
                {
                    c->getOldInfo()->setState(Kernel::CellState::Alive);
                    c->getNewInfo()->setState(Kernel::CellState::Alive);
                    alive++;
                }
            }
        }
        Q_EMIT signalFillingProgress(h + 1);
    }

    m_FieldInformation->upAge();
    m_FieldInformation->setDeadCells(m_FieldInformation->getCellsCount() - alive - cursed);
    m_FieldInformation->setAliveCells(alive);
    m_FieldInformation->setCursedCells(cursed);
    m_FieldInformation->setActiveCells(0);

    qDebug() << "Field" << objectName() << ":" << m_Height * m_Width <<
                "cells filled in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

void Field::calculate()
{
    qDebug() << "Field calculating started";
    while(m_Calculating)
    {
        if(m_WaitScene)
        {
            qDebug() << "Field waits for scene ready...";
            continue;
        }
        m_WaitScene = true;

        auto time = QDateTime::currentMSecsSinceEpoch();
        uint alive_count = 0;
        uint cursed_count = 0;
        uint active_count = 0;
        QVector<Cell*> cells_to_redraw;
        QVector<Cell*> cells_changed;

        if(m_RuleOn) m_FieldInformation->upAge();
        for(int h = 0; h < m_Height; h++)
        {
            if(m_AbortCalculating) break;
            for(int w = 0; w < m_Width; w++)
            {
                if(m_AbortCalculating) break;

                auto c = m_Cells.at(w).at(h);
                auto oi = c->getOldInfo();
                auto ni = c->getNewInfo();

                if(m_RuleOn)
                {
                    applyRules(c);

                    // итог применения правила к ячейке
                    // cell Age
                    if(oi->getState() == Kernel::CellState::Alive) ni->upAge();
                    else ni->setAge(0);

                    // cell Generation
                    if(oi->getAge() == 0 && ni->getAge() > 0) ni->upGeneration();
                }

                auto ois = oi->getState();
                auto nis = ni->getState();

                // если и старое и новое состояние - мёртвое, то ячейка не менялась
                if(nis != Kernel::CellState::Dead || ois != Kernel::CellState::Dead)
                    cells_changed.append(c);

                // Field Information

                if(oi->getGeneration() < ni->getGeneration()) active_count++;

                //Kernel::CellState::Dead не считаем
                if(nis == Kernel::CellState::Alive)
                {
                    cells_to_redraw.append(c);
                    alive_count++;
                }
                else if(nis == Kernel::CellState::Cursed)
                {
                    cells_to_redraw.append(c);
                    cursed_count++;
                }
            }
        }

        if(m_RuleOn) // apply changes
        {
            for(auto c: cells_changed)
            {
                if(m_AbortCalculating) break;
                c->applyInfo();
            }
        }

        m_FieldInformation->setDeadCells(m_FieldInformation->getCellsCount() - alive_count - cursed_count);
        m_FieldInformation->setAliveCells(alive_count);
        m_FieldInformation->setCursedCells(cursed_count);
        m_FieldInformation->setActiveCells(active_count);
        m_FieldInformation->applyAverageCalc(time);
        m_FieldInformation->setChangedCells(cells_changed.count());

        Q_EMIT signalCalculated(cells_to_redraw);

        if(!alive_count) m_CalculatingNonstop = false;

        if(!m_CalculatingNonstop) slotStopCalculating();

        // пауза
        if(m_CalculatingNonstop && m_RuleOn)
        {
            auto pausetime = QDateTime::currentDateTime().addMSecs(config->SceneCalculatingMinPause());
            while(QDateTime::currentDateTime() < pausetime && !m_AbortCalculating)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
    }

    qDebug() << "Field calculating stopped";
    Q_EMIT signalCalculatingDone();
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

void Field::applyRules(Cell *cell)
{
    auto oi = cell->getOldInfo();
    auto ni = cell->getNewInfo();

    // {ActivityType, SelfState, ActivityTarget, TargetState, ActivityOperand, ActivityOperator, [значение]};
    for(auto a: m_Rule->getActivity())
    {
        auto sstate = static_cast<Kernel::CellState>(a.value(1).toInt());   // self state
        if(oi->getState() != sstate) continue;

        auto atype = static_cast<Kernel::ActivityType>(a.value(0).toInt());
        auto atarget = static_cast<Kernel::ActivityTarget>(a.value(2).toInt());
        auto tstate = static_cast<Kernel::CellState>(a.value(3).toInt());
        auto aoperand = static_cast<Kernel::ActivityOperand>(a.value(4).toInt());
        auto aoperator = static_cast<Kernel::ActivityOperator>(a.value(5).toInt());
        auto value =  a.value(6).toUInt();
        auto list = getCellsAroundByStatus(cell, tstate);

        // применение оператора к операнду
        switch(aoperator)
        {
        case Kernel::ActivityOperator::Equal:
        {
            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getOldInfo()->getAge();
                if(count == value) setRulesActivityReaction(ni, atype, list);
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = getRulesOperandValue(aoperand, list);
                if(count == value) setRulesActivityReaction(ni, atype, list);
            }
            break;
        }
        case Kernel::ActivityOperator::Less:
        {
            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getOldInfo()->getAge();
                if(count < value) setRulesActivityReaction(ni, atype, list);
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = getRulesOperandValue(aoperand, list);
                if(count < value) setRulesActivityReaction(ni, atype, list);
            }
            break;
        }
        case Kernel::ActivityOperator::More:
        {
            if(atarget == Kernel::ActivityTarget::Self)
            {
                auto count = cell->getOldInfo()->getAge();
                if(count > value) setRulesActivityReaction(ni, atype, list);
            }
            else if(atarget == Kernel::ActivityTarget::Near)
            {
                auto count = getRulesOperandValue(aoperand, list);
                if(count > value) setRulesActivityReaction(ni, atype, list);
            }
            break;
        }
        default: break;
        }

        if(m_Rule->isDeathEnd() && ni->getState() != Kernel::CellState::Alive) return;
    }
}

uint Field::getRulesOperandValue(Kernel::ActivityOperand ao, QVector<Cell*> list)
{
    uint count = 0;
    if(ao == Kernel::ActivityOperand::Count)
    { count = list.count(); }

    else if(ao == Kernel::ActivityOperand::Age)
    { for(auto c: list) count += c->getOldInfo()->getAge(); }

    return count;
}

void Field::setRulesActivityReaction(CellInformation*ci, Kernel::ActivityType at, QVector<Cell *> list)
{
    if(at == Kernel::ActivityType::Birth)
    { ci->setState(Kernel::CellState::Alive); }

    else if(at == Kernel::ActivityType::Death)
    { ci->setState(Kernel::CellState::Dead); }

    else if(at == Kernel::ActivityType::Bomb)
    { for(auto c: list) c->getOldInfo()->setState(Kernel::CellState::Cursed); }
}

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
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getTopRightCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getRightCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getBottomRightCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getBottomCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getBottomLeftCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getLeftCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    nc = getTopLeftCell(cell);
    if(nc->getOldInfo()->getState() == status) result.append(nc);

    return result;
}

void Field::setCalculatingNonstop(bool value)
{
    if(m_CalculatingNonstop == value) return;

    m_CalculatingNonstop = value;
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

void Field::slotStartCalculating()
{
    m_Calculating = true;
    Q_EMIT signalCalculating(m_Calculating);
}

void Field::slotStopCalculating()
{
    m_Calculating = false;
    Q_EMIT signalCalculating(m_Calculating);
}

FieldRule *Field::getRule() const { return m_Rule; }
Cell *Field::getCell(QPoint index) { return m_Cells.at(index.x()).at(index.y()); }
QVector<QVector<Cell *>> *Field::cells() const { return const_cast<QVector<QVector<Cell*>>*>(&m_Cells); }
int Field::height() { return m_Height; }
int Field::width() { return m_Width; }
bool Field::isCalculating() { return m_Calculating; }
FieldInformation *Field::getInformation() const { return m_FieldInformation; }
void Field::slotSceneReady() { m_WaitScene = false; }
bool Field::isWaitScene() const { return m_WaitScene; }
void Field::AbortCalculating() { m_AbortCalculating = true; }
void Field::setRuleOn(bool value) { m_RuleOn = value; }
