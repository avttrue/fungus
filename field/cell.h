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
    explicit Cell(QObject *parent = nullptr);
    QPoint getIndex();
    void setIndex(const QPoint &value);
    CellInformation* getInformation();
    void clear();
    Field *getField() const;
    void setField(Field *value);

private:
    Field* m_Field;
    QPoint m_Index;
    CellInformation* m_Information;
};

#endif // CELL_H
