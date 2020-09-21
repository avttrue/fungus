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
    void setDefault();                      // Conway's LIFE game
    Activity getActivity() const;
    int getCurseTime() const;
    QString PropertiesToString();
    QString toString();
    QString getDescription() const;

public Q_SLOTS:
    void setActivity(Activity value);
    void setCurseTime(int value);
    void setDescription(const QString& value);

private:
    Activity m_Activity;                    // активность ячейки
    int m_CurseTime;                        // время отравления ячейки (0 - не отравляется, -1 - вечно отравленая)
    QString m_Description;                  // описание правила
};



#endif // FIELDRULE_H
