#ifndef FIELDINFORMATION_H
#define FIELDINFORMATION_H

#include <QObject>

class FieldInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint CellsCount READ getCellsCount WRITE setCellsCount)
    Q_PROPERTY(uint Age READ getAge WRITE setAge NOTIFY signalAgeChanged)
    Q_PROPERTY(uint DeadCells READ getDeadCells WRITE setDeadCells NOTIFY signalDeadCellsChanged)
    Q_PROPERTY(uint AliveCells READ getAliveCells WRITE setAliveCells NOTIFY signalAliveCellsChanged)
    Q_PROPERTY(uint CursedCells READ getCursedCells WRITE setCursedCells NOTIFY signalCursedCellsChanged)
    Q_PROPERTY(uint CellsWithTrait READ getCellsWithTrait WRITE setCellsWithTrait NOTIFY signalCellsWithTraitChanged)
    Q_PROPERTY(qreal Density READ getDensity WRITE setDensity)
    Q_PROPERTY(uint ActiveCells READ getActiveCells WRITE setActiveCells NOTIFY signalActiveCellsChanged)
    Q_PROPERTY(uint LastActiveAge READ getLastActiveAge WRITE setLastActiveAge NOTIFY signalLastActiveAgeChanged)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    uint getAge() const;    
    uint upAge();
    qreal getAverageCalc() const;
    void setAverageCalc(qreal value);
    void applyAverageCalc(uint time);
    uint getDeadCells() const;    
    uint getAliveCells() const;    
    uint getCursedCells() const;        
    uint getActiveCells() const;       
    uint getLastActiveAge() const;    
    qreal getDensity() const;    
    void applyDensity();
    uint getCellsCount() const;
    uint getCellsWithTrait() const;

public Q_SLOTS:
    void setCellsWithTrait(uint value);
    void setCellsCount(const uint &value);
    void setDensity(qreal value);
    void setLastActiveAge(uint value);
    void setActiveCells(uint value);
    void setCursedCells(uint value);
    void setAliveCells(uint value);
    void setDeadCells(uint value);
    void setAge(uint value);

private:
    uint m_CellsCount;
    uint m_Age;
    qreal m_AverageCalc;
    uint m_DeadCells;
    uint m_AliveCells;
    uint m_CursedCells;
    uint m_ActiveCells;
    uint m_LastActiveAge;
    qreal m_Density;        
    uint m_CellsWithTrait;

Q_SIGNALS:
    void signalAgeChanged(uint value);
    void signalAverageCalcChangedUp(qreal value);
    void signalAverageCalcChangedDown(qreal value);
    void signalAverageCalcChanged(qreal value);
    void signalDensityChanged(qreal value);
    void signalDeadCellsChanged(uint value);
    void signalAliveCellsChanged(uint value);
    void signalCursedCellsChanged(uint value);
    void signalActiveCellsChanged(uint value);
    void signalLastActiveAgeChanged(uint value);
    void signalCellsWithTraitChanged(uint value);
};

#endif // FIELDINFORMATION_H
