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

void Field::fill(int random)
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
            if(random && rg.bounded(0, 100) < random)
            {
                c->getOldInfo()->setState(Kernel::CellState::ALIVE);
                c->getNewInfo()->setState(Kernel::CellState::ALIVE);
                alive++;
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
    qDebug() << "Field calculating begin";
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
                    if(oi->getState() == Kernel::CellState::ALIVE &&
                            ni->getState() == Kernel::CellState::CURSED)
                        ni->setAge(0); // была жива, стала отравлена
                    else if(oi->getState() == Kernel::CellState::CURSED &&
                            m_Rule->getCurseTime() > -1)
                        ni->upAge(); // время отравления
                    else if(ni->getState() == Kernel::CellState::ALIVE)
                        ni->upAge(); // возраст живой ячейки
                    else
                        ni->setAge(0); // для всех остальных случаев возраст обнуляется

                    // cell Generation
                    // поколение увеличивается, если только что стала живой
                    if(ni->getState() == Kernel::CellState::ALIVE &&
                            oi->getAge() == 0 &&
                            ni->getAge() > 0) ni->upGeneration();

                    else if(ni->getState() == Kernel::CellState::CURSED) ni->setGeneration(0);
                }

                auto ois = oi->getState();
                auto nis = ni->getState();

                // если и старое и новое состояние - мёртвое, то ячейка не менялась
                if(nis != Kernel::CellState::DEAD || ois != Kernel::CellState::DEAD)
                    cells_changed.append(c);

                // Field Information

                if(oi->getGeneration() < ni->getGeneration()) active_count++;

                //Kernel::CellState::Dead не считаем
                if(nis == Kernel::CellState::ALIVE)
                {
                    cells_to_redraw.append(c);
                    alive_count++;
                }
                else if(nis == Kernel::CellState::CURSED)
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
            // статистика актуальная при включённых правилах
            m_FieldInformation->setActiveCells(active_count);
            m_FieldInformation->setChangedCells(cells_changed.count());
        }

        m_FieldInformation->setDeadCells(m_FieldInformation->getCellsCount() - alive_count - cursed_count);
        m_FieldInformation->setAliveCells(alive_count);
        m_FieldInformation->setCursedCells(cursed_count);
        m_FieldInformation->applyAverageCalc(time);

        Q_EMIT signalCalculated(cells_to_redraw);

        if(!alive_count && (!cursed_count || (cursed_count && m_Rule->getCurseTime() < 0)))
            m_CalculatingNonstop = false;


        if(!m_CalculatingNonstop) slotStopCalculating();

        // пауза
        if(m_CalculatingNonstop && m_RuleOn)
        {
            auto pausetime = QDateTime::currentDateTime().addMSecs(config->SceneCalculatingMinPause());
            while(QDateTime::currentDateTime() < pausetime && !m_AbortCalculating)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
    }

    qDebug() << "Field calculating end";
    Q_EMIT signalCalculatingDone();
}

Cell *Field::addCell(int x, int y)
{
    auto c = new Cell(this);
    c->moveToThread(thread()); c->setParent(this); // NOTE: field выполняется не в основном потоке
    c->setPosition({x, y});
    c->setObjectName(QString("[%1X%2]").arg(QString::number(x), QString::number(y)));
    m_Cells[x][y] = c;
    return c;
}

void Field::applyRules(Cell *cell)
{
    auto oi = cell->getOldInfo();
    auto ni = cell->getNewInfo();

    // проверка возраста отравленной ячейки
    if(oi->getState() == Kernel::CellState::CURSED &&
            m_Rule->getCurseTime() > -1 &&
            oi->getAge() >= static_cast<uint>(m_Rule->getCurseTime()))
        ni->setState(Kernel::CellState::DEAD);

    /* ActivityType,
     * SelfState,
     * ActivityTarget,
     * TargetState,
     * ActivityOperand,
     * ActivityOperator,
     * ActivityValue */
    for(auto a: m_Rule->getActivity())
    {
        auto s_stat = static_cast<Kernel::CellState>(a.value(1).toInt());   // SelfState
        if(oi->getState() != s_stat) continue;

        auto a_type = static_cast<Kernel::ActivityType>(a.value(0).toInt()); // ActivityType
        auto a_targ = static_cast<Kernel::ActivityTarget>(a.value(2).toInt()); // ActivityTarget
        auto t_stat = static_cast<Kernel::CellState>(a.value(3).toInt()); // TargetState
        auto a_opnd = static_cast<Kernel::ActivityOperand>(a.value(4).toInt()); // ActivityOperand
        auto a_oper = static_cast<Kernel::ActivityOperator>(a.value(5).toInt()); // ActivityOperator
        auto a_valu =  a.value(6).toUInt(); // ActivityValue
        uint value; // реальное значение операнда

        // получение реального значения операнда
        QVector<Cell*> at_cells;
        switch(a_targ)
        {
        case Kernel::ActivityTarget::SELF: // tstate игнорируется
        {
            at_cells.append(cell);
            break;
        }
        case Kernel::ActivityTarget::NEAR:
        {
            at_cells.append(getCellsAroundByStatus(cell, t_stat));
            break;
        }
        case Kernel::ActivityTarget::GROUP:
        {
            at_cells.append(getCellsGroupByStatus(cell, t_stat));
            break;
        }
        case Kernel::ActivityTarget::TOP:
        {
            auto c = getTopCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::TOPRIGHT:
        {
            auto c = getTopRightCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::RIGHT:
        {
            auto c = getRightCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::BOTTOMRIGHT:
        {
            auto c = getBottomRightCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::BOTTOM:
        {
            auto c = getBottomCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::BOTTOMLEFT:
        {
            auto c = getBottomLeftCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::LEFT:
        {
            auto c = getLeftCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        case Kernel::ActivityTarget::TOPLEFT:
        {
            auto c = getTopLeftCell(cell);
            if(c->getOldInfo()->getState() == t_stat) at_cells.append(c);
            break;
        }
        }
        value = getRulesOperandValue(a_opnd, at_cells);

        // применение оператора к операнду
        bool ao_check;
        switch(a_oper)
        {
        case Kernel::ActivityOperator::EQUAL:
        { ao_check = value == a_valu ? true : false; break; }
        case Kernel::ActivityOperator::LESS:
        { ao_check = value < a_valu ? true : false; break; }
        case Kernel::ActivityOperator::MORE:
        { ao_check = value > a_valu ? true : false; break; }
        case Kernel::ActivityOperator::NOT:
        { ao_check = value != a_valu ? true : false; break; }
        }
        if(ao_check)
        {

            setRulesActivityReaction(ni, a_type);
            return;
        }
    }
}

uint Field::getRulesOperandValue(Kernel::ActivityOperand ao, QVector<Cell*> list)
{
    uint count = 0;
    switch(ao)
    {
    case Kernel::ActivityOperand::COUNT: // количество соседей определённого типа
    {
        count = list.count(); break;
    }
    case Kernel::ActivityOperand::AGE: // суммарный возраст соседей определённого типа
    {
        for(auto c: list) count += c->getOldInfo()->getAge();
        break;
    }
    case Kernel::ActivityOperand::GEN: // суммарное кол-во поколений соседей определённого типа
    {
        for(auto c: list) count += c->getOldInfo()->getGeneration();
        break;
    }
    }
    return count;
}

void Field::setRulesActivityReaction(CellInformation*ci, Kernel::ActivityType at)
{
    switch(at)
    {
    case Kernel::ActivityType::BIRTH:
    { ci->setState(Kernel::CellState::ALIVE);  break; }
    case Kernel::ActivityType::DEATH:
    { ci->setState(Kernel::CellState::DEAD);  break; }
    case Kernel::ActivityType::CURSE:
    { ci->setState(Kernel::CellState::CURSED);  break; }
    case Kernel::ActivityType::NOTHING:
    { break; }
    }
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

QVector<Cell *> Field::getCellsGroupByStatus(Cell *cell, Kernel::CellState status)
{
    QVector<Cell*> result = getCellsAroundByStatus(cell, status);
    if(cell->getOldInfo()->getState() == status) result.append(cell);
    return result;
}

void Field::setCalculatingNonstop(bool value)
{
    if(m_CalculatingNonstop == value) return;
    m_CalculatingNonstop = value;
    qDebug() << "Field calculating nonstop:" << m_CalculatingNonstop;
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
    qDebug() << "Field calculating started";
}

void Field::slotStopCalculating()
{
    m_Calculating = false;
    Q_EMIT signalCalculating(m_Calculating);
    qDebug() << "Field calculating stopped";
}

void Field::setRuleOn(bool value)
{
    m_RuleOn = value;
    qDebug() << "Field rules on:" << m_RuleOn;
}

void Field::AbortCalculating()
{
    m_AbortCalculating = true;
    qDebug() << "Field calculating aborted";
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


