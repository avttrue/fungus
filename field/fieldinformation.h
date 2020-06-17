#ifndef FIELDINFORMATION_H
#define FIELDINFORMATION_H

#include <QObject>

class FieldInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 Age READ getAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qint64 Calc READ getCalc WRITE setCalc NOTIFY signalCalcChanged)
    Q_PROPERTY(qreal AverageCalc READ getAverageCalc NOTIFY signalAverageCalcChanged)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    qint64 getAge() const;
    qint64 stepAge();
    qint64 getCalc() const;
    void setCalc(qint64 time);
    qreal getAverageCalc() const;

private:
    qint64 m_Age;
    qint64 m_Calc;
    qreal m_AverageCalc;

Q_SIGNALS:
    void signalAgeChanged(qint64 value);
    void signalCalcChanged(qint64 value);
    void signalAverageCalcChanged(qreal value);
};

#endif // FIELDINFORMATION_H
