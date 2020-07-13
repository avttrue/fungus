#ifndef FIELD_H
#define FIELD_H

#include <QObject>

#include <kernel/kernel.h>

class Cell;
class FieldRule;
class FieldInformation;

class Field : public QObject
{
    Q_OBJECT

public:
    Field(int width, int height, QObject *parent = nullptr);
    void fill();
    int width();
    int height();
    Cell* addCell(int x, int y);
    Cell* addCell(QPoint index);
    Cell* getCell(QPoint index);
    QVector<QVector<Cell *>>* cells() const; 
    FieldRule *getRule() const;
    void setRule(FieldRule *value);
    void setRunning(bool value);
    bool isRunning();
    void calculate();
    bool getRunningAlways() const;
    void setRunningAlways(bool value);
    FieldInformation *getInformation() const;
    void StopCalculating();
    bool getRuleOn() const;
    void setRuleOn(bool value);
    qint64 getCellsCount() const;

protected:
    Cell* getTopCell(Cell* cell);
    Cell* getLeftCell(Cell* cell);
    Cell* getRightCell(Cell* cell);
    Cell* getBottomCell(Cell* cell);
    Cell* getTopLeftCell(Cell* cell);
    Cell* getTopRightCell(Cell* cell);
    Cell* getBottomLeftCell(Cell* cell);
    Cell* getBottomRightCell(Cell* cell);
    QVector<Cell*> getCellsAround(Cell* cell);
    QVector<Cell*> getCellsAroundByStatus(Cell* cell, Kernel::CellState status);
    void applyCalculating();                        // применение результатов calculate
    void applyRules(Cell* cell);                       // применение правил
    void testRules(Cell* c);

private:
    int m_Width;
    int m_Height;
    qint64 m_CellsCount;
    QVector<QVector<Cell*>> m_Cells;
    FieldRule* m_Rule;
    FieldInformation* m_FieldInformation;
    bool m_RuleOn;                                  // включить расчёт правил (для отрисовки при редактировании)
    bool m_Running;                                 // флаг управления циклом calculate
    bool m_RunningAlways;                           // флаг управления режимом calculate: одиночный шаг / постоянное выполнение
    bool m_WaitScene;                               // ожидание готовности сцены
    bool m_StopCalculating;                         // остановка цикла calculate перед выходом и т.д.

public Q_SLOTS:
    void slotSceneReady();

Q_SIGNALS:
    void signalRuleChanged(FieldRule* rule);         // правила изменены
    void signalRunning(bool value);                 // состояние вкл/выкл цикла calculate
    void signalCalculatingStopped();                // calculate остановлен/завершён
    void signalCalculated(QVector<Cell*> cells);    // завершена итерация calculate
    void signalFillingProgress(int step);           // для индикации создания
};

#endif // FIELD_H
