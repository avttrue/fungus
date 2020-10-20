#include "snapshotlist.h"

#include <QDebug>

SnapshotList::SnapshotList(QObject *parent)
    : QObject(parent)
{
    setObjectName("SnapshotList");
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    Q_EMIT signalCountChanged(0);
    qDebug() << objectName() << "created";
}

void SnapshotList::clear()
{
    m_Snapshots.clear();
    Q_EMIT signalCountChanged(0);
    qDebug() << objectName() << "cleared";
}

void SnapshotList::clearAfter(const QString &key)
{
    qDebug() << __func__;
    int pos = -1;
    for(int i = 0; i < m_Snapshots.count(); i++)
    {
        if(m_Snapshots.at(i).first == key)
        {
            pos = i;
            break;
        }
    }

    if(pos <= -1)
    {
      qDebug() << "Key not found:" <<  key;
      return;
    }
    if(pos >= m_Snapshots.count() - 1)
    {
      qDebug() << "Key is last:" <<  key;
      return;
    }

    m_Snapshots.remove(pos + 1, m_Snapshots.count() - pos - 1);
    Q_EMIT signalCountChanged(m_Snapshots.count());
    qDebug() << objectName() << "count:" << m_Snapshots.count();
}

void SnapshotList::addItem(const QString& key, Snapshot snapshot)
{
    qDebug() << __func__;
    int pos = -1;
    for(int i = 0; i < m_Snapshots.count(); i++)
    {
        if(m_Snapshots.at(i).first == key)
        {
            pos = i;
            qDebug() << "Snapshot already in list, rewriting";
            break;
        }
    }
    if(pos >= 0) m_Snapshots.replace(pos, {key, snapshot});
    else m_Snapshots.append({key, snapshot});
    Q_EMIT signalCountChanged(m_Snapshots.count());
    qDebug() << objectName() << "count:" << m_Snapshots.count();
}

Snapshot SnapshotList::getSnapshot(const QString &key)
{
    qDebug() << __func__;
    if(m_Snapshots.isEmpty())
    {
        qDebug() << "List is empty";
        return Snapshot();
    }

    for(auto value: m_Snapshots)
        if(value.first == key) return value.second;

    qCritical() << "Key not found:" << key;
    return Snapshot();
}


Snapshot SnapshotList::getFirst()
{
    qDebug() << __func__;
    if(m_Snapshots.isEmpty())
    {
        qDebug() << "List is empty";
        return Snapshot();
    }

    return m_Snapshots.at(0).second;
}

QStringList SnapshotList::keys()
{
    QStringList list;
    for(auto value: m_Snapshots) list.append(value.first);
    return list;
}

int SnapshotList::count() { return m_Snapshots.count(); }



