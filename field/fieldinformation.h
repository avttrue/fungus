#ifndef FIELDINFORMATION_H
#define FIELDINFORMATION_H

#include <QObject>

class FieldInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 Age READ getAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qint64 DeadCells READ getDeadCells WRITE setDeadCells)
    Q_PROPERTY(qint64 AliveCells READ getAliveCells WRITE setAliveCells)
    Q_PROPERTY(qint64 CursedCells READ getCursedCells WRITE setCursedCells)
    Q_PROPERTY(qreal AverageCalc READ getAverageCalc WRITE applyAverageCalc)
    Q_PROPERTY(qint64 ActiveCells READ getActiveCells WRITE setActiveCells)
    Q_PROPERTY(qint64 LastActiveAge READ getLastActiveAge WRITE setLastActiveAge)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    qint64 getAge() const;
    qint64 upAge();
    qreal getAverageCalc() const;
    void applyAverageCalc(qint64 time);
    qint64 getDeadCells() const;
    void setDeadCells(qint64 value);
    qint64 getAliveCells() const;
    void setAliveCells(qint64 value);
    qint64 getCursedCells() const;
    void setCursedCells(qint64 value);    
    qint64 getActiveCells() const;
    void setActiveCells(qint64 value);   
    qint64 getLastActiveAge() const;
    void setLastActiveAge(qint64 value);

private:
    qint64 m_Age;
    qreal m_AverageCalc;
    qint64 m_DeadCells;
    qint64 m_AliveCells;
    qint64 m_CursedCells;

    qint64 m_ActiveCells;

    qint64 m_LastActiveAge;

Q_SIGNALS:
    void signalAgeChanged(qint64 value);
    void signalAverageCalcChangedUp(qreal value);
    void signalAverageCalcChangedDown(qreal value);
    void signalDeadCellsChanged(qint64 value);
    void signalAliveCellsChanged(qint64 value);
    void signalCursedCellsChanged(qint64 value);
    void signalActiveCellsChanged(qint64 value);
    void signalLastActiveAgeChanged(qint64 value);
};

#endif // FIELDINFORMATION_H
