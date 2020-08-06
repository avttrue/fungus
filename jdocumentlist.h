#ifndef JDOCUMENTLIST_H
#define JDOCUMENTLIST_H

#include <QJsonDocument>
#include <QObject>

class JDocumentList : public QObject
{
    Q_OBJECT
public:
    explicit JDocumentList(QObject *parent = nullptr);
    void clearList();
    void addDocument(const QString& key, QJsonDocument document);
    QJsonDocument getDocument(const QString& key);
    QStringList keys();
    int count();

private:
    QVector<QPair<QString, QJsonDocument>> m_List;

Q_SIGNALS:

};

#endif // JDOCUMENTLIST_H
