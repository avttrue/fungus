#include "field.h"
#include "fieldrule.h"
#include "helper.h"

#include <QDebug>

// TODO: Временное исполнение FieldRule
FieldRule::FieldRule(Field *parent)
    : QObject(parent),
      m_CurseTime(0),
      m_DeathEnd(true)
{
    setDefault(); // TODO: setDefault убрать

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellRule" << objectName() <<"destroyed"; });
    qDebug() << "CellRule" << objectName() << "created";
}

void FieldRule::setActivity(Activity value)
{
    if (m_Activity == value) return;
    m_Activity = value;
}

void FieldRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;
    m_CurseTime = value;
}

void FieldRule::setDeathEnd(bool value)
{
    if (m_DeathEnd == value) return;
    m_DeathEnd = value;
}

void FieldRule::setDefault()
{
    setObjectName(tr("Conway's LIFE game"));

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::BIRTH),
                        QVariant::fromValue(Kernel::CellState::DEAD),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::EQUAL),
                        3});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::DEATH),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::LESS),
                        2});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::DEATH),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::MORE),
                        3});
}

Activity FieldRule::getActivity() const { return m_Activity; }
int FieldRule::getCurseTime() const { return m_CurseTime; }
bool FieldRule::isDeathEnd() const { return m_DeathEnd; }
