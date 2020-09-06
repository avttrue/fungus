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
    QRect getRect() const;
    bool isObserved();
    void setObserved(bool value);

private:
    Field* m_Field;
    QPoint m_Index;
    CellInformation* m_OldInformation;
    CellInformation* m_NewInformation;
    QRect m_Rect;
    bool m_Observed; // флаг того, что клетка под наблюдением

};

#endif // CELL_H
