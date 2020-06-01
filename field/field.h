#ifndef FIELD_H
#define FIELD_H

#include <QObject>

class Cell;

struct FieldInformation
{

};

class Field : public QObject
{
    Q_OBJECT
public:
    Field(QObject *parent,
          int width,
          int height);
    Cell* addCell(int x, int y);
    int Width() const;
    int Height() const;
    QVector<QVector<Cell *>>* Cells();

protected:

private:
    FieldInformation m_Information;
    int m_Width;
    int m_Height;
    QVector<QVector<Cell*>> m_Cells;

Q_SIGNALS:
    void signalCellAdded(Cell* cell);
    void signalProgress(int progress);

};

#endif // FIELD_H
