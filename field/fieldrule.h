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
    Q_PROPERTY(QString Description READ getDescription WRITE setDescription)

public:
    FieldRule(Field *parent = nullptr);
    FieldRule(FieldRule* rule, Field *parent = nullptr);
    Activity getActivity() const;
    void setDefault();                      // Conway's LIFE game
    void setActivity(Activity value);
    int getCurseTime() const;
    void setCurseTime(int value);
    QString PropertiesToString();
    QString toString();    
    QString getDescription() const;
    void setDescription(const QString& value);

protected:

private:
    Activity m_Activity;                    // активность ячейки
    int m_CurseTime;                        // время отравления ячейки (0 - не отравляется, -1 - вечно отравленая)   
    QString m_Description;
};



#endif // FIELDRULE_H
