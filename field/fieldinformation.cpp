#include "fieldinformation.h"
#include "helper.h"

#include <QDateTime>
#include <QDebug>

FieldInformation::FieldInformation(QObject *parent)
    : QObject(parent),
      m_Age(-1),
      m_AverageCalc(0),
      m_DeadCells(-1),
      m_AliveCells(-1),
      m_CursedCells(-1)
{
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "FieldInformation destroyed"; });
}


qint64 FieldInformation::stepAge()
{
    m_Age++;

    Q_EMIT signalAgeChanged(m_Age);
    return m_Age;
}

void FieldInformation::applyAverageCalc(qint64 time)
{
    auto new_c = QDateTime::currentMSecsSinceEpoch() - time;

    qreal new_ac = calcAverage(m_AverageCalc, m_Age, new_c);

    if (m_AverageCalc > new_ac || m_AverageCalc < new_ac)
    {
        m_AverageCalc = new_ac;
        Q_EMIT signalAverageCalcChanged(m_AverageCalc);
    }
}

void FieldInformation::setDeadCells(qint64 value)
{
    if (m_DeadCells == value) return;

    m_DeadCells = value;
    Q_EMIT signalDeadCellsChanged(m_DeadCells);
}

void FieldInformation::setAliveCells(qint64 value)
{
    if (m_AliveCells == value) return;

    m_AliveCells = value;
    Q_EMIT signalAliveCellsChanged(m_AliveCells);
}

void FieldInformation::setCursedCells(qint64 value)
{
    if (m_CursedCells == value) return;

    m_CursedCells = value;
    Q_EMIT signaCursedCellsChanged(m_CursedCells);
}

qreal FieldInformation::getAverageCalc() const { return m_AverageCalc; }
qint64 FieldInformation::getAge() const { return m_Age; }
qint64 FieldInformation::getDeadCells() const { return m_DeadCells; }
qint64 FieldInformation::getAliveCells() const { return m_AliveCells; }
qint64 FieldInformation::getCursedCells() const { return m_CursedCells; }
