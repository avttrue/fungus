#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>

class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    explicit Cell(Field *parent);
    QPoint getIndex();
    void setIndex(const QPoint &value);
    CellInformation* getCurInfo();
    CellInformation *getNewInfo();
    void clear();
    Field *getField() const;
    void applyNewInfo();

private:
    Field* m_Field;
    QPoint m_Index;
    CellInformation* m_CurrentInformation;
    CellInformation* m_NewInformation;

};

#endif // CELL_H
