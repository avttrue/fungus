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
    if(m_List.contains(key))
        qDebug() << __FILE__  << __func__ << "Document already in list";

    m_List.insert(key, document);
    qDebug() << objectName() << "count:" << m_List.count();
}

QJsonDocument JDocumentList::getDocument(const QString &key)
{
    if(m_List.isEmpty())
    {
        qDebug() << __FILE__  << __func__ << "List is empty";
        return QJsonDocument();
    }

    if(!m_List.contains(key))
    {
        qCritical() << __FILE__ << __func__ << "Key not found:" << key;
        return QJsonDocument();
    }

    return m_List.value(key);
}

QMap<QString, QJsonDocument> *JDocumentList::getList() { return &m_List; }
