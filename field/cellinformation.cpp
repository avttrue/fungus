#include "cellinformation.h"
#include "cell.h"

#include <QDebug>

CellInformation::CellInformation(Cell *parent)
    : QObject(parent),
      m_Age(0),
      m_Generation(0),
      m_State(Kernel::CellState::Dead)
{

}

void CellInformation::setState(Kernel::CellState value)
{
    if (m_State == value) return;

    m_State = value;
    Q_EMIT signalStateChanged(m_State);
}

void CellInformation::setAge(qint64 value)
{
    if (m_Age == value) return;

    m_Age = value;
    Q_EMIT signalAgeChanged(m_Age);
}

void CellInformation::setGeneration(qint64 value)
{
    if (m_Generation == value) return;

    m_Generation = value;
    Q_EMIT signalGenerationChanged(m_Generation);
}

Kernel::CellState CellInformation::getState() const { return m_State; }
qint64 CellInformation::getAge() const { return m_Age; }
qint64 CellInformation::getGeneration() const { return m_Generation; }
