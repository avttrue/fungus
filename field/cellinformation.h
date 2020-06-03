#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include <QObject>
#include <QVariant>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool Alive READ getAlive WRITE setAlive NOTIFY signalChanged)
    Q_PROPERTY(int Age READ getAge WRITE setAge NOTIFY signalChanged)
    Q_PROPERTY(int Generation READ getGeneration WRITE setGeneration NOTIFY signalChanged)

public:
    CellInformation(Cell *parent);
    bool getAlive() const;
    int getAge() const;
    int getGeneration() const;
    void setAlive(bool value);
    void setAge(int value);
    void setGeneration(int value);
    QMap<QString, QVariant::Type> getPropertiesList();

private:
    int m_Age;
    int m_Generation;
    bool m_Alive;

Q_SIGNALS:
    void signalChanged();

};

#endif // CELLINFORMATION_H
