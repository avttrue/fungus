#include "jdocumentlist.h"

#include <QDebug>

JDocumentList::JDocumentList(QObject *parent)
    : QObject(parent),
      m_Position(-1)
{
    setObjectName("JsonDocumentList");
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void JDocumentList::clearList()
{
    m_List.clear();
    m_Position = -1;
    Q_EMIT signalPositionChanged(m_Position);
    qDebug() << objectName() << "cleared";
}

void JDocumentList::addDocument(QJsonDocument document)
{
    m_List.append(document);
    m_Position = m_List.count() - 1;
    Q_EMIT signalPositionChanged(m_Position);
    qDebug() << objectName() << "count:" << m_List.count();
}

QJsonDocument JDocumentList::getDocument(int position)
{
    if(m_List.isEmpty())
    {
        qDebug() << __FILE__  << __func__ << "List is empty";
        Q_EMIT signalPositionChanged(-1);
        return QJsonDocument();
    }

    if(position == -1) return m_List.at(m_Position);

    if(position < 0 || position >= m_List.count())
    {
        qCritical() << __FILE__ << __func__ << "Invalid position:" << position;
        Q_EMIT signalPositionChanged(m_Position);
        return QJsonDocument();
    }

    m_Position = position;
    Q_EMIT signalPositionChanged(m_Position);
    return m_List.at(position);
}

QJsonDocument JDocumentList::getNextDocument()
{
    if(m_List.isEmpty())
    {
        qDebug() << __FILE__  << __func__ << "List is empty";
        Q_EMIT signalPositionChanged(-1);
        return QJsonDocument();
    }

    m_Position++;
    if(m_Position >= m_List.count() - 1) m_Position = m_List.count() - 1;

    Q_EMIT signalPositionChanged(m_Position);
    return m_List.at(m_Position);
}

QJsonDocument JDocumentList::getPrevDocument()
{
    if(m_List.isEmpty())
    {
        qDebug() << __FILE__  << __func__ << "List is empty";
        Q_EMIT signalPositionChanged(-1);
        return QJsonDocument();
    }

    m_Position--;
    if(m_Position < 0) m_Position = 0;

    Q_EMIT signalPositionChanged(m_Position);
    return m_List.at(m_Position);
}

QList<QJsonDocument> *JDocumentList::getDocumentList() { return &m_List; }
int JDocumentList::getPosition() const { return m_Position; }
int JDocumentList::getCount() const { return m_List.count(); }
