#include "field.h"
#include "cellrule.h"
#include "helper.h"

#include <QDebug>

CellRule::CellRule(Field *parent)
    : QObject(parent),
      m_ColorAlive(Qt::black),
      m_CurseTime(0)
{
    setDefault();

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellRule" << objectName() <<"destroyed"; });
    qDebug() << "CellRule" << objectName() << "created";
}

void CellRule::setColorAlive(QColor value)
{
    if (m_ColorAlive == value) return;

    m_ColorAlive = value;
    Q_EMIT signalChanged();
}

void CellRule::setActivity(CellActivity value)
{
    if (m_Activity == value) return;

    m_Activity = value;
    Q_EMIT signalChanged();
}

void CellRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;

    m_CurseTime = value;
    Q_EMIT signalChanged();
}

void CellRule::setDefault()
{
    setObjectName(tr("Conway's LIFE game"));

    m_Activity.append({QVariant::fromValue(Kernel::CellActivityType::Birth),
                       QVariant::fromValue(Kernel::CellActivityTarget::Near),
                       QVariant::fromValue(Kernel::CellActivityOperator::Equal), 3});
    m_Activity.append({QVariant::fromValue(Kernel::CellActivityType::Death),
                       QVariant::fromValue(Kernel::CellActivityTarget::Near),
                       QVariant::fromValue(Kernel::CellActivityOperator::Less), 2});
    m_Activity.append({QVariant::fromValue(Kernel::CellActivityType::Death),
                       QVariant::fromValue(Kernel::CellActivityTarget::Near),
                       QVariant::fromValue(Kernel::CellActivityOperator::More), 3});
}

QColor CellRule::getColorAlive() const { return m_ColorAlive; }
CellActivity CellRule::getActivity() const { return m_Activity; }
int CellRule::getCurseTime() const { return m_CurseTime; }
