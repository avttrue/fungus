#include "fieldservice.h"

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
        qCritical() << "Wrong enum name: '" << enumname << "'";
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
    QMetaEnum me = Kernel::staticMetaObject.enumerator(index);

    return me.keyCount();
}
