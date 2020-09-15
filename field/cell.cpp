#include "cell.h"
#include "cellinformation.h"
#include "field.h"
#include "helper.h"
#include "properties.h"

#include <QDebug>

Cell::Cell(Field *parent)
    : QObject(parent),
      m_Field(parent),
      m_Index(-1, -1),
      m_Rect(-1, -1, -1, -1),
      m_Observed(false)
{
    m_OldInformation = new CellInformation(this);
    m_NewInformation = new CellInformation(this);
}

void Cell::clear()
{
    m_NewInformation->setAge(0);
    m_NewInformation->setState(Kernel::CellState::DEAD);
    m_NewInformation->setGeneration(0);

    applyInfo();
}

void Cell::applyInfo()
{
    m_OldInformation->setAge(m_NewInformation->getAge());
    m_OldInformation->setState(m_NewInformation->getState());
    m_OldInformation->setGeneration(m_NewInformation->getGeneration());
}

void Cell::flipInfo(const QPoint &value)
{
    auto c = getField()->getCell(value);

    m_NewInformation->setAge(c->m_OldInformation->getAge());
    m_NewInformation->setState(c->m_OldInformation->getState());
    m_NewInformation->setGeneration(c->m_OldInformation->getGeneration());

    c->m_NewInformation->setAge(m_OldInformation->getAge());
    c->m_NewInformation->setState(m_OldInformation->getState());
    c->m_NewInformation->setGeneration(m_OldInformation->getGeneration());

    applyInfo();
    c->applyInfo();
}

void Cell::setPosition(const QPoint &value)
{
    m_Index = value;

    auto glw = config->SceneGridLineWidth();
    auto cs = config->SceneCellSize();
    m_Rect = QRect(value.x() * (cs + glw) + glw,
                   value.y() * (cs + glw) + glw,
                   cs, cs);
}

QRect Cell::getRect() const { return m_Rect; }
bool Cell::isObserved() { return m_Observed; }

void Cell::setObservedOn()
{
    m_Observed = true;
    qDebug() << "Cell" << objectName() << ": Observed on";
}

void Cell::setObservedOff()
{
    m_Observed = false;
    qDebug() << "Cell" << objectName() << ": Observed off";
}
QPoint Cell::getIndex() { return m_Index; }
CellInformation *Cell::getOldInfo() { return m_OldInformation; }
CellInformation *Cell::getNewInfo() { return m_NewInformation; }
Field *Cell::getField() const { return m_Field; }
