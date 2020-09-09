#include "fieldinformation.h"
#include "helper.h"

#include <QDateTime>
#include <QDebug>

FieldInformation::FieldInformation(QObject *parent)
    : QObject(parent),
      m_CellsCount(0),
      m_Age(-1),
      m_AverageCalc(0),
      m_DeadCells(-1),
      m_AliveCells(-1),
      m_CursedCells(-1),
      m_ActiveCells(-1),
      m_Density(0)
{
    setObjectName("FieldInformation");
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
}


uint FieldInformation::upAge()
{
    m_Age++;

    Q_EMIT signalAgeChanged(m_Age);
    return m_Age;
}

void FieldInformation::applyAverageCalc(uint time)
{
    auto new_c = QDateTime::currentMSecsSinceEpoch() - time;

    qreal new_ac = calcAverage(m_AverageCalc, m_Age, new_c);

    if (m_AverageCalc > new_ac || m_AverageCalc < new_ac)
    {
        auto up = false;
        if(m_AverageCalc < new_ac) up = true;
        m_AverageCalc = new_ac;

        if(up) Q_EMIT signalAverageCalcChangedUp(m_AverageCalc);
        else Q_EMIT signalAverageCalcChangedDown(m_AverageCalc);
    }
}

void FieldInformation::setDeadCells(uint value)
{
    if (m_DeadCells == value) return;

    m_DeadCells = value;
    Q_EMIT signalDeadCellsChanged(m_DeadCells);
    applyDensity();
}

void FieldInformation::setAliveCells(uint value)
{
    if (m_AliveCells == value) return;

    m_AliveCells = value;
    Q_EMIT signalAliveCellsChanged(m_AliveCells);
    applyDensity();
}

void FieldInformation::setCursedCells(uint value)
{
    if (m_CursedCells == value) return;

    m_CursedCells = value;
    Q_EMIT signalCursedCellsChanged(m_CursedCells);
    applyDensity();
}

void FieldInformation::setActiveCells(uint value)
{
    if (m_ActiveCells == value) return;

    setLastActiveAge(m_Age);

    m_ActiveCells = value;
    Q_EMIT signalActiveCellsChanged(m_ActiveCells);
    applyDensity();
}

void FieldInformation::setLastActiveAge(uint value)
{
    if (m_LastActiveAge == value) return;

    m_LastActiveAge = value;
    Q_EMIT signalLastActiveAgeChanged(m_LastActiveAge);
}

void FieldInformation::applyDensity()
{
    qreal value = static_cast<qreal>(m_AliveCells) /
            (m_DeadCells + m_CursedCells + m_AliveCells);
    if (m_Density == value) return;

    m_Density = value;
    Q_EMIT signalDensityChanged(m_Density);
}

void FieldInformation::setCellsCount(const uint &value) { m_CellsCount = value; }

void FieldInformation::setAge(uint value)
{
    if (m_Age == value) return;

    m_Age = value;
    Q_EMIT signalAgeChanged(value);
}

void FieldInformation::setDensity(qreal value)
{
    if (m_Density == value) return;

    m_Density = value;
    Q_EMIT signalDensityChanged(m_Density);
}

void FieldInformation::setAverageCalc(qreal value)
{
   if (m_AverageCalc == value) return;

    m_AverageCalc = value;
    Q_EMIT signalAverageCalcChanged(m_AverageCalc);
}
qreal FieldInformation::getDensity() const { return m_Density; }
uint FieldInformation::getLastActiveAge() const { return m_LastActiveAge; }
uint FieldInformation::getActiveCells() const { return m_ActiveCells; }
qreal FieldInformation::getAverageCalc() const { return m_AverageCalc; }
uint FieldInformation::getAge() const { return m_Age; }
uint FieldInformation::getDeadCells() const { return m_DeadCells; }
uint FieldInformation::getAliveCells() const { return m_AliveCells; }
uint FieldInformation::getCursedCells() const { return m_CursedCells; }
uint FieldInformation::getCellsCount() const { return m_CellsCount; }
