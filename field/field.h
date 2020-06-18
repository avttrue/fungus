#ifndef FIELD_H
#define FIELD_H

#include <QMap>
#include <QObject>

class Cell;
class CellRule;
class FieldInformation;

class Field : public QObject
{
    Q_OBJECT
public:
    Field(int width, int height, QObject *parent = nullptr);
    int width();
    int height();
    Cell* addCell(int x, int y);
    Cell* addCell(QPoint index);
    Cell* getCell(QPoint index);
    QVector<QVector<Cell *>>* cells() const; 
    CellRule *getRule() const;
    void setRule(CellRule *value);
    void setRunning(bool value);
    bool isRunning();
    void calculate();
    bool getRunningAlways() const;
    void setRunningAlways(bool value);
    FieldInformation *getFieldInfo() const;
    void setWaitScene(bool value);
    void StopCalculating();

protected:
    Cell* getTopCell(Cell* c);
    Cell* getLeftCell(Cell* c);
    Cell* getRightCell(Cell* c);
    Cell* getBottomCell(Cell* c);
    Cell* getTopLeftCell(Cell* c);
    Cell* getTopRightCell(Cell* c);
    Cell* getBottomLeftCell(Cell* c);
    Cell* getBottomRightCell(Cell* c);
    QVector<Cell*> getCellsAround(Cell* c);

private:
    int m_Width;
    int m_Height;
    QVector<QVector<Cell*>> m_Cells;
    CellRule* m_Rule;
    FieldInformation* m_FieldInformation;
    bool m_Running;                             // флаг управления циклом calculate
    bool m_RunningAlways;                       // флаг управления режимом calculate: одиночный шаг / постоянное выполнение
    bool m_WaitScene;                           // ожидание готовности сцены
    bool m_StopCalculating;                     // остановка цикла calculate перед выходом и т.д.


Q_SIGNALS:
    void signalRuleChanged(CellRule* rule);     // правила изменены
    void signalRunning(bool value);             // состояние вкл/выкл цикла calculate
    void signalCalculatingStopped();            // calculate остановлен/завершён
    void signalCalculated(QSet<Cell*> cells);   // завершена итерация calculate
};

#endif // FIELD_H
