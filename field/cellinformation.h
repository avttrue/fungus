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
    Q_PROPERTY(bool Active READ isActive WRITE setActive NOTIFY signalActivityChanged)
    Q_PROPERTY(int CursedAge READ getCursedAge WRITE setCursedAge NOTIFY signalCursedAgeChanged)

public:
    CellInformation(Cell *parent);
    Kernel::CellState getState() const;
    uint getAge() const;
    uint getGeneration() const;
    void setState(Kernel::CellState value);
    void setAge(uint value);
    void upAge();
    void setGeneration(uint value);
    void upGeneration();    
    bool isActive() const;
    void setActive(bool value);    
    int getCursedAge() const;
    void setCursedAge(int CursedAge);

private:
    uint m_Age;                   // возраст живой ячейки
    uint m_Generation;            // поколение ячейки (увеличивается при смерти-рождении)
    Kernel::CellState m_State;      // сострояние ячейки
    bool m_Active;                  // активность ячейки (изменяется при росте поколения)
    int m_CursedAge;                // учёт времени отравления (согласно FieldRule)

Q_SIGNALS:
    void signalStateChanged(QVariant value);
    void signalAgeChanged(uint value);
    void signalGenerationChanged(uint value);
    void signalActivityChanged(bool Active);
    void signalCursedAgeChanged(int CursedAge);
};

#endif // CELLINFORMATION_H
