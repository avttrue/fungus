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
    void addDocument(QJsonDocument document);
    QJsonDocument getDocument(int position = -1);
    QJsonDocument getNextDocument();
    QJsonDocument getPrevDocument();
    QList<QJsonDocument>* getDocumentList();
    int getPosition() const;
    int getCount() const;

private:
    QList<QJsonDocument> m_List;
    int m_Position;

Q_SIGNALS:
    void signalPositionChanged(int value);
};

#endif // JDOCUMENTLIST_H
