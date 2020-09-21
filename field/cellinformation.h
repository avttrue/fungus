#ifndef CELLINFORMATION_H
#define CELLINFORMATION_H

#include "kernel/kernel.h"

#include <QObject>

class Cell;

class CellInformation : public QObject
{
    Q_OBJECT
    // properties
    Q_PROPERTY(Kernel::CellState State READ getState WRITE setState NOTIFY signalStateChanged)
    Q_PROPERTY(bool Trait READ isTrait WRITE setTrait NOTIFY signalTraitChanged)
    // statistics
    Q_PROPERTY(uint Age READ getAge WRITE setAge NOTIFY signalAgeChanged)
    Q_PROPERTY(uint Generation READ getGeneration WRITE setGeneration NOTIFY signalGenerationChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    uint getAge() const;
    uint getGeneration() const;
    bool isTrait() const;
    void upAge();
    void downAge();    
    void upGeneration();

public Q_SLOTS:
    void setState(Kernel::CellState value);
    void setAge(uint value);
    void setGeneration(uint value);
    void setTrait(bool value);

private:
    Kernel::CellState m_State;    // сострояние ячейки
    bool m_Trait;                 // особенность ячейки
    uint m_Age;                   // возраст живой ячейки
    uint m_Generation;            // поколение ячейки (увеличивается при смерти-рождении)    

Q_SIGNALS:
    void signalStateChanged(QVariant value);
    void signalAgeChanged(uint value);
    void signalGenerationChanged(uint value);
    void signalTraitChanged(bool value);
};

#endif // CELLINFORMATION_H
