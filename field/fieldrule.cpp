#include "field.h"
#include "fieldrule.h"
#include "helper.h"

#include <QDebug>

FieldRule::FieldRule(Field *parent)
    : QObject(parent),
      m_ColorAlive(Qt::black),
      m_CurseTime(0)
{
    setDefault();

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellRule" << objectName() <<"destroyed"; });
    qDebug() << "CellRule" << objectName() << "created";
}

void FieldRule::setColorAlive(QColor value)
{
    if (m_ColorAlive == value) return;

    m_ColorAlive = value;
    Q_EMIT signalChanged();
}

void FieldRule::setActivity(Activity value)
{
    if (m_Activity == value) return;

    m_Activity = value;
    Q_EMIT signalChanged();
}

void FieldRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;

    m_CurseTime = value;
    Q_EMIT signalChanged();
}

void FieldRule::setDefault()
{
    setObjectName(tr("Conway's LIFE game"));

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::Birth),
                        QVariant::fromValue(Kernel::CellState::Dead),
                        QVariant::fromValue(Kernel::ActivityTarget::Near),
                        QVariant::fromValue(Kernel::CellState::Alive),
                        QVariant::fromValue(Kernel::ActivityOperand::Count),
                        QVariant::fromValue(Kernel::ActivityOperator::Equal),
                        3});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::Death),
                        QVariant::fromValue(Kernel::CellState::Alive),
                        QVariant::fromValue(Kernel::ActivityTarget::Near),
                        QVariant::fromValue(Kernel::CellState::Alive),
                        QVariant::fromValue(Kernel::ActivityOperand::Count),
                        QVariant::fromValue(Kernel::ActivityOperator::Less),
                        2});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::Death),
                        QVariant::fromValue(Kernel::CellState::Alive),
                        QVariant::fromValue(Kernel::ActivityTarget::Near),
                        QVariant::fromValue(Kernel::CellState::Alive),
                        QVariant::fromValue(Kernel::ActivityOperand::Count),
                        QVariant::fromValue(Kernel::ActivityOperator::More),
                        3});
}

QColor FieldRule::getColorAlive() const { return m_ColorAlive; }
Activity FieldRule::getActivity() const { return m_Activity; }
int FieldRule::getCurseTime() const { return m_CurseTime; }
