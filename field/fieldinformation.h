#ifndef FIELDINFORMATION_H
#define FIELDINFORMATION_H

#include <QObject>

class FieldInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 Age READ getAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qint64 DeadCells READ getDeadCells WRITE setDeadCells NOTIFY signalDeadCellsChanged)
    Q_PROPERTY(qint64 AliveCells READ getAliveCells WRITE setAliveCells NOTIFY signalAliveCellsChanged)
    Q_PROPERTY(qint64 CursedCells READ getCursedCells WRITE setCursedCells NOTIFY signaCursedCellsChanged)
    Q_PROPERTY(qreal AverageCalc READ getAverageCalc WRITE applyAverageCalc NOTIFY signalAverageCalcChanged)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    qint64 getAge() const;
    qint64 stepAge();
    qreal getAverageCalc() const;
    void applyAverageCalc(qint64 time);    
    qint64 getDeadCells() const;
    void setDeadCells(qint64 DeadCells);
    qint64 getAliveCells() const;
    void setAliveCells(qint64 AliveCells);
    qint64 getCursedCells() const;
    void setCursedCells(qint64 CursedCells);

private:
    qint64 m_Age;
    qreal m_AverageCalc;    
    qint64 m_DeadCells;

    qint64 m_AliveCells;

    qint64 m_CursedCells;

Q_SIGNALS:
    void signalAgeChanged(qint64 value);
    void signalAverageCalcChanged(qreal value);
    void signalDeadCellsChanged(qint64 DeadCells);
    void signalAliveCellsChanged(qint64 AliveCells);
    void signaCursedCellsChanged(qint64 CursedCells);
};

#endif // FIELDINFORMATION_H
