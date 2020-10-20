#ifndef SNAPSHOTLIST_H
#define SNAPSHOTLIST_H

#include <QJsonDocument>
#include <QObject>
#include <QPixmap>

struct Snapshot
{
    QJsonDocument document = QJsonDocument();
    QPixmap pixmap = QPixmap();
};

class SnapshotList : public QObject
{
    Q_OBJECT
public:
    explicit SnapshotList(QObject *parent = nullptr);
    void clear();
    void clearAfter(const QString& key);
    void addItem(const QString& key, Snapshot snapshot);
    Snapshot getSnapshot(const QString& key);
    Snapshot getFirst();
    QStringList keys();
    int count();

private:
    QVector<QPair<QString, Snapshot>> m_Snapshots;

Q_SIGNALS:
    void signalCountChanged(int count);

};

#endif // SNAPSHOTLIST_H
