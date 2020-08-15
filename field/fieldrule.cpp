#include "field.h"
#include "fieldrule.h"
#include "helper.h"

#include <QDebug>
#include <QMetaProperty>

// TODO: Временное исполнение FieldRule
FieldRule::FieldRule(Field *parent)
    : QObject(parent),
      m_CurseTime(0),
      m_DeathEnd(true)
{
    setDefault(); // TODO: setDefault убрать

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "FieldRule" << objectName() <<"destroyed"; });
    qDebug() << "FieldRule" << objectName() << "created";
}

void FieldRule::setActivity(Activity value)
{
    if (m_Activity == value) return;
    m_Activity = value;
    qDebug() << "FieldRule" << objectName() << "changed";
}

void FieldRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;
    m_CurseTime = value;
    qDebug() << "FieldRule" << objectName() << "CurseTime changed:" << value;
}

void FieldRule::setDeathEnd(bool value)
{
    if (m_DeathEnd == value) return;
    m_DeathEnd = value;
    qDebug() << "FieldRule" << objectName() << "DeathEnd changed:" << value;
}

QString FieldRule::PropertiesToString()
{
    QString result = tr("Name : %1").arg(objectName());
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        auto p = metaObject()->property(i);
        auto value = property(p.name());
        if(QString(p.name()) != "Activity")
          result.append(QString("\n%1 : %2").arg(p.name(), value.toString()));
    }
    return result;
}

QString FieldRule::toString()
{
   QString result = PropertiesToString();
   result.append(tr("\nActivities:"));
   for(auto a: m_Activity)
      result.append(QString("\n%1").arg(ActivityElementToString(a)));
   return result;
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
