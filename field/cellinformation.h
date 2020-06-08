#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include "fieldservice.h"

#include <QObject>
#include <QVariant>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Kernel::CellState State READ getState WRITE setState NOTIFY signalChanged)
    Q_PROPERTY(int Age READ getAge WRITE setAge NOTIFY signalChanged)
    Q_PROPERTY(int Generation READ getGeneration WRITE setGeneration NOTIFY signalChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    int getAge() const;
    int getGeneration() const;
    void setState(Kernel::CellState value);
    void setAge(int value);
    void setGeneration(int value);

private:
    int m_Age;
    int m_Generation;
    Kernel::CellState m_State;

Q_SIGNALS:
    void signalChanged();

};

#endif // CELLINFORMATION_H
