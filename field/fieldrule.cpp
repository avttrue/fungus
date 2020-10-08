#include "field.h"
#include "fieldrule.h"
#include "helpers/helper.h"
#include "properties.h"

#include <QDebug>
#include <QMetaProperty>

FieldRule::FieldRule(Field *parent)
    : QObject(parent),
      m_CurseTime(0),
      m_Description("")
{
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "FieldRule" << objectName() <<"destroyed"; });
    QObject::connect(this, &QObject::objectNameChanged, [=](){ qDebug() << "FieldRule: name changed to" << objectName(); });
    qDebug() << "FieldRule created";
}

FieldRule::FieldRule(FieldRule *rule, Field *parent)
    : QObject(parent)
{
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "FieldRule" << objectName() <<"destroyed"; });
    QObject::connect(this, &QObject::objectNameChanged, [=](){ qDebug() << "FieldRule: name changed to" << objectName(); });

    if(!rule)
    {
        qCritical() << __func__ << "Source rule is null";
        return;
    }

    setObjectName(rule->objectName());
    auto r_mo = rule->metaObject();
    for(int i = r_mo->propertyOffset(); i < r_mo->propertyCount(); ++i)
    {
        auto p = r_mo->property(i);
        auto value = rule->property(p.name());
        setProperty(p.name(), value);
    }

    qDebug() << "FieldRule" << objectName() << "created as copy";
}

void FieldRule::setActivity(Activity value)
{
    if (m_Activity == value) return;
    m_Activity = value;
    qDebug() << "FieldRule" << objectName() << "Activity changed:" << m_Activity;
}

void FieldRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;
    m_CurseTime = value;
    qDebug() << "FieldRule" << objectName() << "CurseTime changed:" << m_CurseTime;
}

QString FieldRule::PropertiesToString()
{
    QString result = tr("Name : \"%1\"").arg(objectName());
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        auto p = metaObject()->property(i);
        auto value = property(p.name());
        auto s_value = value.type() == QVariant::Bool
                ? BoolToString(value.toBool())
                : value.toString();
        if(QString(p.name()) != "Activity")
            result.append(QString("\n%1 : %2").arg(p.name(), s_value.leftRef(FIELD_RULE_PROPERTY_LENGTH)));
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
    setDescription("B3/S23");

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::BIRTH),
                        QVariant::fromValue(Kernel::CellState::DEAD),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::EQUAL),
                        3, true});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::DEATH),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::LESS),
                        2, true});

    m_Activity.append({ QVariant::fromValue(Kernel::ActivityType::DEATH),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                        QVariant::fromValue(Kernel::CellState::ALIVE),
                        QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                        QVariant::fromValue(Kernel::ActivityOperator::MORE),
                        3, true});

    qDebug() << "FieldRule" << objectName() << "filled as default";
}

Activity FieldRule::getActivity() const { return m_Activity; }
int FieldRule::getCurseTime() const { return m_CurseTime; }
QString FieldRule::getDescription() const { return m_Description; }
void FieldRule::setDescription(const QString &value) { m_Description = value; }
