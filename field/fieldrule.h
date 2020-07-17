#ifndef FIELDRULE_H
#define FIELDRULE_H

#include "kernel/kernel.h"

#include <QColor>
#include <QObject>

class Field;

class FieldRule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor ColorAlive READ getColorAlive WRITE setColorAlive)
    Q_PROPERTY(Activity Activity READ getActivity WRITE setActivity)
    Q_PROPERTY(int CurseTime READ getCurseTime WRITE setCurseTime)
    Q_PROPERTY(bool DeathEnd READ isDeathEnd WRITE setDeathEnd)

public:
    FieldRule(Field *parent = nullptr);
    QColor getColorAlive() const;
    void setColorAlive(QColor value);
    Activity getActivity() const;
    void setActivity(Activity value);
    int getCurseTime() const;
    void setCurseTime(int value); 
    bool isDeathEnd() const;
    void setDeathEnd(bool value);

protected:
    void setDefault();                      // Conway's LIFE game

private:
    QColor m_ColorAlive;                    // цвет живой ячейки
    Activity m_Activity;                    // активность ячейки
    int m_CurseTime;                        // время отравления ячейки после смерти или взрыва (0 - не отравляется)   
    bool m_DeathEnd;                        // смерть ячейки прекращает обработку активность

Q_SIGNALS:

};



#endif // FIELDRULE_H
