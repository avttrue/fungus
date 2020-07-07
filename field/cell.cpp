#include "cell.h"
#include "cellinformation.h"

#include <QDebug>
#include <scene/sceneItem.h>

Cell::Cell(QObject *parent)
    : QObject(parent),
    m_Field(nullptr),
    m_Index({-1, -1})
{
    m_Information = new CellInformation(this);
}

void Cell::clear()
{
    m_Information->setAge(0);
    m_Information->setState(Kernel::CellState::Dead);
    m_Information->setGeneration(0);
}

QPoint Cell::getIndex() { return m_Index; }
void Cell::setIndex(const QPoint &value) { m_Index = value; }
CellInformation *Cell::getInformation() { return m_Information; }
Field *Cell::getField() const { return m_Field; }
void Cell::setField(Field *value) { m_Field = value; }
