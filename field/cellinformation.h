#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include "fieldservice.h"

#include <QObject>
#include <QVariant>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Kernel::CellState State READ getState WRITE setState NOTIFY signalStateChanged)
    Q_PROPERTY(qint64 Age READ getAge WRITE setAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qint64 Generation READ getGeneration WRITE setGeneration NOTIFY signalGenerationChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    qint64 getAge() const;
    qint64 getGeneration() const;
    void setState(Kernel::CellState value);
    void setAge(qint64 value);
    void setGeneration(qint64 value);

private:
    qint64 m_Age;
    qint64 m_Generation;
    Kernel::CellState m_State;

Q_SIGNALS:
    void signalStateChanged(Kernel::CellState value);
    void signalAgeChanged(qint64 value);
    void signalGenerationChanged(qint64 value);
};

#endif // CELLINFORMATION_H
