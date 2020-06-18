#ifndef FIELDINFORMATION_H
#define FIELDINFORMATION_H

#include <QObject>

class FieldInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 Age READ getAge NOTIFY signalAgeChanged)
    Q_PROPERTY(qreal AverageCalc READ getAverageCalc NOTIFY signalAverageCalcChanged)

public:
    explicit FieldInformation(QObject *parent = nullptr);

    qint64 getAge() const;
    qint64 stepAge();
    qreal getAverageCalc() const;
    void applyAverageCalc(qint64 time);

private:
    qint64 m_Age;
    qreal m_AverageCalc;

Q_SIGNALS:
    void signalAgeChanged(qint64 value);
    void signalAverageCalcChanged(qreal value);
};

#endif // FIELDINFORMATION_H
