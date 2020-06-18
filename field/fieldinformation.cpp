#include "fieldinformation.h"
#include "helper.h"

#include <QDateTime>
#include <QDebug>

FieldInformation::FieldInformation(QObject *parent)
    : QObject(parent),
      m_Age(0),
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

qreal FieldInformation::getAverageCalc() const { return m_AverageCalc; }
qint64 FieldInformation::getAge() const { return m_Age; }
