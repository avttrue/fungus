#include "kernel.h"

#include <QDebug>
#include <QMetaProperty>
#include <QObject>

QString getNameKernelEnum(const QString &enumname, int index)
{
    QStringList list = listKernelEnum(enumname);
    if(list.count() <= index)
    {
        qCritical() << "Wrong index: '" << index << "' for '" << enumname << "' enum; count" << list.count();
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
        qCritical() << "Wrong enum name:" << enumname;
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
        qCritical() << "Wrong enum name:" << enumname;
        return -1;
    }

    QMetaEnum me = Kernel::staticMetaObject.enumerator(index);

    return me.keyCount();
}

QString ActivityElementToString(const QVector<QVariant> &activity)
{
    /* {ActivityType,
     * SelfState,
     * ActivityTarget,
     * TargetState,
     * ActivityOperand,
     * ActivityOperator,
     * [значение]}*/
    auto activitytype = getNameKernelEnum("ActivityType", activity.at(0).toInt());
    auto activitysstate = getNameKernelEnum("CellState", activity.at(1).toInt());
    auto activitytarget = getNameKernelEnum("ActivityTarget", activity.at(2).toInt());
    auto activitytstate = getNameKernelEnum("CellState", activity.at(3).toInt());
    auto activityoperand = getNameKernelEnum("ActivityOperand", activity.at(4).toInt());
    auto activityoperator = getNameKernelEnum("ActivityOperator", activity.at(5).toInt());
    auto activityvalue = activity.at(6).toString();

    return QObject::tr("%1 IF CELL IS %2 AND %3 IS %4 AND %5 %6 %7").
            arg(activitytype,
                activitysstate,
                activitytarget,
                activitytstate,
                activityoperand,
                activityoperator,
                activityvalue);
}
