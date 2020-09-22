#include "kernel.h"

#include <QDebug>
#include <QIcon>
#include <QMetaProperty>
#include <QObject>

QString getNameKernelEnum(const QString &enumname, int index)
{
    QStringList list = listKernelEnum(enumname);
    if(list.count() <= index)
    {
        qCritical() << __func__ << "Wrong index: '" << index << "' for '" << enumname << "' enum; count" << list.count();
        return "";
    }

    return list.at(index);
}

QStringList listKernelEnum(const QString &enumname)
{
    QStringList list;
    const char* en = enumname.toLatin1().data();
    int index = Kernel::staticMetaObject.indexOfEnumerator(en);

    if(index == -1)
    {
        qCritical() << __func__ << "Wrong enum name:" << enumname;
        return list;
    }

    QMetaEnum me = Kernel::staticMetaObject.enumerator(index);

    for(int i =0; i < me.keyCount(); i++) list.append(me.key(i));

    return list;
}

int countKernelEnum(const QString &enumname)
{
    const char* en = enumname.toLatin1().data();
    int index = Kernel::staticMetaObject.indexOfEnumerator(en);

    if(index == -1)
    {
        qCritical() << __func__ << "Wrong enum name:" << enumname;
        return -1;
    }

    QMetaEnum me = Kernel::staticMetaObject.enumerator(index);

    return me.keyCount();
}

QString ActivityElementToString(const QVector<QVariant> &activity)
{
    /* { ActivityType,
     * SelfState,
     * ActivityTarget,
     * TargetState,
     * ActivityOperand,
     * ActivityOperator,
     * ActivityValue,
     * Abort } */
    auto activitytype = QVariant::fromValue(activity.at(0)).toString();
    auto activitysstate = QVariant::fromValue(activity.at(1)).toString();
    auto activitytarget = QVariant::fromValue(activity.at(2)).toString();
    auto activitytstate =
            static_cast<Kernel::ActivityTarget>(activity.at(2).toInt()) == Kernel::ActivityTarget::SELF
            ? "****" : QVariant::fromValue(activity.at(3)).toString();
    auto activityoperand = QVariant::fromValue(activity.at(4)).toString();
    auto activityoperator = QVariant::fromValue(activity.at(5)).toString();
    auto activityvalue = activity.at(6).toString();
    auto activitybreak = activity.at(7).toBool() ? "ABORT" : "CONTINUE";

    return QObject::tr("set %1 if cell is %2 and %3 is { %4 %5 %6 %7 } and then %8").
            arg(activitytype,
                activitysstate,
                activitytarget,
                activitytstate,
                activityoperand,
                activityoperator,
                activityvalue,
                activitybreak);
}

QIcon ActivityTypeToIcon(Kernel::ActivityType type)
{
    QIcon icon;
    switch (type)
    {
    case Kernel::ActivityType::BIRTH:
    { icon = QIcon(":/resources/img/cell.svg"); break; }
    case Kernel::ActivityType::DEATH:
    { icon = QIcon(":/resources/img/cell_dead.svg"); break; }
    case Kernel::ActivityType::CURSE:
    { icon = QIcon(":/resources/img/cell_curse.svg"); break; }
    case Kernel::ActivityType::NOTHING:
    { icon = QIcon(":/resources/img/nothing.svg"); break; }
    case Kernel::ActivityType::UP_AGE:
    { icon = QIcon(":/resources/img/up.svg"); break; }
    case Kernel::ActivityType::DOWN_AGE:
    { icon = QIcon(":/resources/img/down.svg"); break; }
    case Kernel::ActivityType::INVERT:
    { icon = QIcon(":/resources/img/sel_unsel.svg"); break; }
    case Kernel::ActivityType::TRAIT:
    { icon = QIcon(":/resources/img/circle_dot.svg"); break; }
    case Kernel::ActivityType::WO_TRAIT:
    { icon = QIcon(":/resources/img/circle.svg"); break; }
    }
    return icon;
}
