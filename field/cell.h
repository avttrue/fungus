#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>
#include <QRect>

class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    explicit Cell(Field *parent);
    QPoint getIndex();
    void setIndex(const QPoint &value);
    CellInformation* getOldInfo();
    CellInformation *getNewInfo();
    void clear();
    Field *getField() const;
    void applyInfo();
    QRect getRect() const;

private:
    Field* m_Field;
    QPoint m_Index;
    CellInformation* m_OldInformation;
    CellInformation* m_NewInformation;
    QRect m_Rect;

};

#endif // CELL_H
