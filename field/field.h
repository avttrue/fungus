#ifndef FIELD_H
#define FIELD_H

#include <QObject>

#include <kernel/kernel.h>

class Cell;
class CellInformation;
class FieldRule;
class FieldInformation;

class Field : public QObject
{
    Q_OBJECT

public:
    Field(int width, int height, QObject *parent = nullptr);
    void fill(bool random = false);
    int width();
    int height();
    Cell* addCell(int x, int y);
    Cell* addCell(QPoint index);
    Cell* getCell(QPoint index);
    QVector<QVector<Cell *>>* cells() const;
    FieldRule *getRule() const;
    void setRule(FieldRule *value);    
    bool isCalculating();
    void calculate();
    void setCalculatingNonstop(bool value);
    FieldInformation *getInformation() const;
    void AbortCalculating();
    void setRuleOn(bool value);
    bool isWaitScene() const;

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
    void applyRules(Cell* cell);                    // применение правил
    uint getRulesOperandValue(Kernel::ActivityOperand ao, QVector<Cell*> list); // получить значение в операнде
    void setRulesActivityReaction(CellInformation *ci, Kernel::ActivityType at, QVector<Cell*> list);


private:
    int m_Width;
    int m_Height;
    QVector<QVector<Cell*>> m_Cells;
    FieldRule* m_Rule;
    FieldInformation* m_FieldInformation;
    bool m_RuleOn;                                  // включить расчёт правил (для отрисовки при редактировании)
    bool m_Calculating;                             // флаг управления циклом calculate
    bool m_CalculatingNonstop;                      // флаг управления режимом calculate: одиночный шаг / постоянное выполнение
    bool m_WaitScene;                               // ожидание готовности сцены
    bool m_AbortCalculating;                        // остановка цикла calculate перед выходом и т.д.

public Q_SLOTS:
    void slotSceneReady();
    void slotStopCalculating();                     // флаг m_Calculating = false
    void slotStartCalculating();                    // флаг m_Calculating = true

Q_SIGNALS:
    void signalRuleChanged(FieldRule* rule);        // правила изменены
    void signalCalculating(bool value);             // состояние вкл/выкл цикла calculate
    void signalCalculatingDone();                   // calculate остановлен/завершён
    void signalCalculated(QVector<Cell*> cells);    // завершена итерация calculate
    void signalFillingProgress(int step);           // для индикации создания
};

#endif // FIELD_H
