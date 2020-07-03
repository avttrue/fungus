#include "kernel.h"

#include <QDebug>
#include <QMetaProperty>

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

QString CellActivityElementToString(const QVector<QVariant> &activity)
{
    if(activity.count() < 4)
    {
        qCritical() << __func__ << "Wrong CellActivity format, count" << activity.count();
        return "";
    }

    auto activitytype = getNameKernelEnum("CellActivityType", activity.at(0).toInt());
    auto activitytarget = getNameKernelEnum("CellActivityTarget", activity.at(1).toInt());
    auto activityoperator = activity.at(2).toString();
    auto activityvalue = activity.at(3).toString();

    return QString("%1 %2 %3 %4").arg(activitytype, activitytarget, activityoperator, activityvalue);
}
