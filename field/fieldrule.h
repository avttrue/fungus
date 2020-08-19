#ifndef FIELDRULE_H
#define FIELDRULE_H

#include "kernel/kernel.h"

#include <QColor>
#include <QObject>

class Field;

class FieldRule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Activity Activity READ getActivity WRITE setActivity)
    Q_PROPERTY(int CurseTime READ getCurseTime WRITE setCurseTime)
    Q_PROPERTY(bool DeathEnd READ isDeathEnd WRITE setDeathEnd)

public:
    FieldRule(Field *parent = nullptr);
    FieldRule(FieldRule* rule, Field *parent = nullptr);
    Activity getActivity() const;
    void setDefault();                      // Conway's LIFE game
    void setActivity(Activity value);
    int getCurseTime() const;
    void setCurseTime(int value);
    bool isDeathEnd() const;
    void setDeathEnd(bool value);
    QString PropertiesToString();
    QString toString();

protected:   

private:
    Activity m_Activity;                    // активность ячейки
    int m_CurseTime;                        // время отравления ячейки (0 - не отравляется, -1 - вечно отравленая)
    bool m_DeathEnd;                        // смерть ячейки прекращает обработку активности

Q_SIGNALS:

};



#endif // FIELDRULE_H
