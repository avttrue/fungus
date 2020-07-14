#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include "kernel/kernel.h"

#include <QObject>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Kernel::CellState State READ getState WRITE setState NOTIFY signalStateChanged)
    Q_PROPERTY(qint64 Age READ getAge WRITE setAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qint64 Generation READ getGeneration WRITE setGeneration NOTIFY signalGenerationChanged)
    Q_PROPERTY(bool Active READ isActive WRITE setActive NOTIFY signalActivityChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    qint64 getAge() const;
    qint64 getGeneration() const;
    void setState(Kernel::CellState value);
    void setAge(qint64 value);
    void upAge();
    void setGeneration(qint64 value);
    void upGeneration();    
    bool isActive() const;
    void setActive(bool value);

private:
    qint64 m_Age;
    qint64 m_Generation;
    Kernel::CellState m_State;    
    bool m_Active;

Q_SIGNALS:
    void signalStateChanged(int value);
    void signalAgeChanged(qint64 value);
    void signalGenerationChanged(qint64 value);
    void signalActivityChanged(bool Active);
};

#endif // CELLINFORMATION_H
