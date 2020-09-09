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
    Q_PROPERTY(qreal Density READ getDensity WRITE setDensity)
    Q_PROPERTY(qreal AverageCalc READ getAverageCalc WRITE setAverageCalc)
    Q_PROPERTY(uint ActiveCells READ getActiveCells WRITE setActiveCells NOTIFY signalActiveCellsChanged)
    Q_PROPERTY(uint LastActiveAge READ getLastActiveAge WRITE setLastActiveAge NOTIFY signalLastActiveAgeChanged)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    uint getAge() const;
    void setAge(uint value);
    uint upAge();
    qreal getAverageCalc() const;
    void setAverageCalc(qreal value);
    void applyAverageCalc(uint time);
    uint getDeadCells() const;
    void setDeadCells(uint value);
    uint getAliveCells() const;
    void setAliveCells(uint value);
    uint getCursedCells() const;
    void setCursedCells(uint value);    
    uint getActiveCells() const;
    void setActiveCells(uint value);   
    uint getLastActiveAge() const;
    void setLastActiveAge(uint value);
    qreal getDensity() const;
    void setDensity(qreal value);
    void applyDensity();
    uint getCellsCount() const;
    void setCellsCount(const uint &value);   

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
};

#endif // FIELDINFORMATION_H
