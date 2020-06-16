#include "fieldinformation.h"

#include <QDateTime>
#include <QDebug>

FieldInformation::FieldInformation(QObject *parent)
    : QObject(parent),
      m_Age(0),
      m_Calc(0),
      m_AverageCalc(0)
{
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "FieldInformation destroyed"; });
}


qint64 FieldInformation::stepAge()
{
    m_Age++;

    Q_EMIT signalAgeChanged(m_Age);
    return m_Age;
}

void FieldInformation::setCalc(qint64 time)
{
    auto new_c = QDateTime::currentMSecsSinceEpoch() - time;

    qreal age_1 = static_cast<qreal>(m_Age) - 1;
    qreal new_ac = m_Age == 1
                       ? new_c
                       : (m_AverageCalc + static_cast<qreal>(new_c) / age_1) /
                             (static_cast<qreal>(m_Age) / age_1);

    if (m_AverageCalc > new_ac || m_AverageCalc < new_ac)
    {
        m_AverageCalc = new_ac;
        Q_EMIT signalAverageCalcChanged(m_AverageCalc);
    }
    if (m_Calc != new_c)
    {
        m_Calc = new_c;
        Q_EMIT signalCalcChanged(m_Calc);
    }
}

qreal FieldInformation::getAverageCalc() const { return m_AverageCalc; }
qint64 FieldInformation::getAge() const { return m_Age; }
qint64 FieldInformation::getCalc() const { return m_Calc; }
