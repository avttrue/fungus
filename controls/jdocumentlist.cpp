#include "jdocumentlist.h"

#include <QDebug>

JDocumentList::JDocumentList(QObject *parent)
    : QObject(parent)
{
    setObjectName("JsonDocumentList");
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void JDocumentList::clearList()
{
    m_List.clear();
    qDebug() << objectName() << "cleared";
}

void JDocumentList::addDocument(const QString& key, QJsonDocument document)
{
    qDebug() << __func__;
    int pos = -1;
    for(int i = 0; i < m_List.count(); i++)
    {
        if(m_List.at(i).first == key)
        {
            pos = i;
            qDebug() << "Document already in list, rewriting";
            break;
        }
    }
    if(pos >= 0) m_List.replace(pos, {key, document});
    else m_List.append({key, document});
    qDebug() << objectName() << "count:" << m_List.count();
}

QJsonDocument JDocumentList::getDocument(const QString &key)
{
    qDebug() << __func__;
    if(m_List.isEmpty())
    {
        qDebug() << "List is empty";
        return QJsonDocument();
    }

    for(auto value: m_List)
        if(value.first == key) return value.second;

    qCritical() << "Key not found:" << key;
    return QJsonDocument();
}

QJsonDocument JDocumentList::getFirstDocument()
{
    qDebug() << __func__;
    if(m_List.isEmpty())
    {
        qDebug() << "List is empty";
        return QJsonDocument();
    }

    return m_List.at(0).second;
}

QStringList JDocumentList::keys()
{
    QStringList list;
    for(auto value: m_List) list.append(value.first);
    return list;
}

int JDocumentList::count() { return m_List.count(); }



