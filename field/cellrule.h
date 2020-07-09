#ifndef CELLRULE_H
#define CELLRULE_H

#include "kernel/kernel.h"

#include <QColor>
#include <QObject>

class Field;

class CellRule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor ColorAlive READ getColorAlive WRITE setColorAlive NOTIFY signalChanged)
    Q_PROPERTY(CellActivity Activity READ getActivity WRITE setActivity NOTIFY signalChanged)
    Q_PROPERTY(int CurseTime READ getCurseTime WRITE setCurseTime NOTIFY signalChanged)

public:
    CellRule(Field *parent = nullptr);
    QColor getColorAlive() const;
    void setColorAlive(QColor value);
    CellActivity getActivity() const;
    void setActivity(CellActivity value);
    int getCurseTime() const;
    void setCurseTime(int value);

protected:
    void setDefault();                      // Conway's LIFE game

private:
    QColor m_ColorAlive;                    // цвет живой ячейки
    CellActivity m_Activity;                // активность ячейки
    int m_CurseTime;                        // время отравления ячейки после смерти или взрыва (0 - не отравляется)

Q_SIGNALS:
    void signalChanged();
};



#endif // CELLRULE_H
