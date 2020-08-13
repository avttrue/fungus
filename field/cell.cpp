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
      m_Rect(-1, -1, -1, -1)
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

void Cell::setIndex(const QPoint &value)
{
    m_Index = value;
    m_Rect = QRect(value.x() * config->SceneCellSize(),
                   value.y() * config->SceneCellSize(),
                   config->SceneCellSize(),
                   config->SceneCellSize());
}

QRect Cell::getRect() const { return m_Rect; }
QPoint Cell::getIndex() { return m_Index; }
CellInformation *Cell::getOldInfo() { return m_OldInformation; }
CellInformation *Cell::getNewInfo() { return m_NewInformation; }
Field *Cell::getField() const { return m_Field; }
