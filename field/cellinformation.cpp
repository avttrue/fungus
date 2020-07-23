#include "cellinformation.h"
#include "cell.h"

#include <QDebug>

CellInformation::CellInformation(Cell *parent)
    : QObject(parent),
      m_Age(0),
      m_Generation(0),
      m_State(Kernel::CellState::Dead),
      m_Active(false),
      m_CursedAge(0)
{ }

void CellInformation::setState(Kernel::CellState value)
{
    if (m_State == value) return;

    m_State = value;
    Q_EMIT signalStateChanged(QVariant::fromValue(m_State));
}

void CellInformation::setAge(uint value)
{
    if (m_Age == value) return;

    m_Age = value;
    Q_EMIT signalAgeChanged(m_Age);
}

void CellInformation::upAge()
{
    m_Age++;
    Q_EMIT signalAgeChanged(m_Age);
}

void CellInformation::setGeneration(uint value)
{
    if (m_Generation == value) return;

    m_Generation = value;
    Q_EMIT signalGenerationChanged(m_Generation);
}

void CellInformation::upGeneration()
{
    m_Generation++;
    Q_EMIT signalGenerationChanged(m_Generation);
}

void CellInformation::setActive(bool value)
{
    if (m_Active == value) return;

    m_Active = value;
    Q_EMIT signalActivityChanged(m_Active);
}

void CellInformation::setCursedAge(int CursedAge)
{
    if (m_CursedAge == CursedAge) return;

    m_CursedAge = CursedAge;
    Q_EMIT signalCursedAgeChanged(m_CursedAge);
}

int CellInformation::getCursedAge() const { return m_CursedAge; }
bool CellInformation::isActive() const { return m_Active; }
Kernel::CellState CellInformation::getState() const { return m_State; }
uint CellInformation::getAge() const { return m_Age; }
uint CellInformation::getGeneration() const { return m_Generation; }
