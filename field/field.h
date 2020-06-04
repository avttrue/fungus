#ifndef FIELD_H
#define FIELD_H

#include <QObject>

class Cell;

class Field : public QObject
{
    Q_OBJECT
public:
    Field(QObject *parent,
          int width,
          int height);
    int width() const;
    int height() const;
    Cell* addCell(int x, int y);
    Cell* getCell(QPoint index) const;
    QVector<QVector<Cell *>>* cells() const;
    Cell* getTopCell(Cell* c) const;
    Cell* getRightCell(Cell* c) const;
    Cell* getBottomCell(Cell* c) const;
    Cell* getLeftCell(Cell* c) const;
    Cell* getTopLeftCell(Cell* c) const;
    Cell* getTopRightCell(Cell* c) const;
    Cell* getBottomLeftCell(Cell* c) const;
    Cell *getBottomRightCell(Cell* c) const;

protected:

private:
    int m_Width;
    int m_Height;
    QVector<QVector<Cell*>> m_Cells;

Q_SIGNALS:
    void signalCellAdded(Cell* cell);
    void signalProgress(int progress);

};

#endif // FIELD_H
