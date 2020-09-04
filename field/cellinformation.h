#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include "kernel/kernel.h"

#include <QObject>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Kernel::CellState State READ getState WRITE setState NOTIFY signalStateChanged)
    Q_PROPERTY(uint Age READ getAge WRITE setAge NOTIFY signalAgeChanged)
    Q_PROPERTY(uint Generation READ getGeneration WRITE setGeneration NOTIFY signalGenerationChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    uint getAge() const;
    uint getGeneration() const;
    void setState(Kernel::CellState value);
    void setAge(uint value);
    void upAge();
    void downAge();
    void setGeneration(uint value);
    void upGeneration();    

private:
    uint m_Age;                   // возраст живой ячейки
    uint m_Generation;            // поколение ячейки (увеличивается при смерти-рождении)
    Kernel::CellState m_State;    // сострояние ячейки

Q_SIGNALS:
    void signalStateChanged(QVariant value);
    void signalAgeChanged(uint value);
    void signalGenerationChanged(uint value);
};

#endif // CELLINFORMATION_H
