#ifndef FIELD_H
#define FIELD_H

#include <QMap>
#include <QObject>

class Cell;
class CellRule;

class Field : public QObject
{
    Q_OBJECT
public:
    Field(QObject *parent, int width, int height);
    void fill();
    int width();
    int height();
    Cell* addCell(int x, int y);
    Cell* addCell(QPoint index);
    Cell* getCell(QPoint index);
    QVector<QVector<Cell *>>* cells() const;
    Cell* getTopCell(Cell* c);
    Cell* getLeftCell(Cell* c);
    Cell* getRightCell(Cell* c);
    Cell* getBottomCell(Cell* c);
    Cell* getTopLeftCell(Cell* c);
    Cell* getTopRightCell(Cell* c);
    Cell* getBottomLeftCell(Cell* c);
    Cell* getBottomRightCell(Cell* c);
    QVector<Cell*> getCellsAround(Cell* c);
    QMap<QString, CellRule *> getCellRules() const;

private:
    int m_Width;
    int m_Height;
    QVector<QVector<Cell*>> m_Cells;
    QMap<QString, CellRule*> m_CellRules;
    QString m_DefaultRule;

Q_SIGNALS:
    void signalCellAdded(Cell* cell);
    void signalProgress(int progress);

};

#endif // FIELD_H
