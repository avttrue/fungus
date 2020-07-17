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
    Q_PROPERTY(int CursedAge READ getCursedAge WRITE setCursedAge NOTIFY signalCursedAgeChanged)

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
    int getCursedAge() const;
    void setCursedAge(int CursedAge);

private:
    qint64 m_Age;                   // возраст живой ячейки
    qint64 m_Generation;            // поколение ячейки (увеличивается при смерти-рождении)
    Kernel::CellState m_State;      // сострояние ячейки
    bool m_Active;                  // активность ячейки (изменяется при росте поколения)
    int m_CursedAge;                // учёт времени отравления (согласно FieldRule)

Q_SIGNALS:
    void signalStateChanged(QVariant value);
    void signalAgeChanged(qint64 value);
    void signalGenerationChanged(qint64 value);
    void signalActivityChanged(bool Active);
    void signalCursedAgeChanged(int CursedAge);
};

#endif // CELLINFORMATION_H
