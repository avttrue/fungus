#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QRect>

class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    explicit Cell(Field *parent);
    QPoint getIndex();
    void setPosition(const QPoint &value);
    CellInformation* getOldInfo();
    CellInformation *getNewInfo();
    void clear();
    Field *getField() const;
    void applyInfo();
    void flipInfo(const QPoint &value);
    QRect getRect() const;
    bool isObserved();

private:
    Field* m_Field;
    QPoint m_Index;
    CellInformation* m_OldInformation;
    CellInformation* m_NewInformation;
    QRect m_Rect;
    bool m_Observed; // флаг того, что клетка под наблюдением

public Q_SLOTS:
    void setObservedOn();
    void setObservedOff();

};

#endif // CELL_H
