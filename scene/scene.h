#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class SceneItem;
class Field;
class Cell;

class Scene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(qreal AverageDraw READ getAverageDraw WRITE applyAverageDraw)

public:
    Scene(QObject *parent, Field* field);
    void addSceneItem();
    void addSelectionMark();
    void addMultiSelectionMark();
    void selectCell(Cell* cell);
    void MultiselectCell(Cell* cell);
    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);
    QSize size() const;
    Field *getField() const;
    void StopAdvanse();
    SceneItem *getSceneItem() const;
    qreal getAverageDraw() const;
    void applyAverageDraw(qint64 time);
    Cell *getSelectedCell() const;
    Cell *getSecondSelectedCell() const;
    void clearMultiSelection();
    void setSelectionMarkColor(const QString& color);
    void setMultiSelectionMarkColor(const QString& color);

private:
    Field* m_Field;
    Cell* m_SelectedCell;                                  // выбранная Cell
    Cell* m_SecondSelectedCell;                            // вторая выбранная Cell для группового выбора
    SceneItem* m_SceneItem;                                // основной объект для отрисовки Field
    QGraphicsRectItem* m_SelectionMark;                    // метка выбраной Cell
    QGraphicsRectItem* m_MultiSelectionMark;               // метка выбраной группы Cell
    QColor m_BackgroundColor;
    QSize m_Size;
    qreal m_AverageDraw;                                   // среднее время отрисовки сцены
    bool m_StopAdvanse;                                    // остановка отрисовки перед выходом и .т.д

public Q_SLOTS:
    void slotAdvance(QVector<Cell*> cells);                // перерисовать сцену

Q_SIGNALS:
    void signalProgress(int progress);                     // прогресс создания сцены
    void signalAverageDrawChangedUp(qreal AverageDraw);    // среднее время отрисовки сцены увеличилось
    void signalAverageDrawChangedDown(qreal AverageDraw);  // среднее время отрисовки сцены уменьшилось
    void signalSelectedCellChanged(Cell* cell);            // выбор Cell
    void signalSelectedCellsChanged(Cell* first, Cell* second);// выбор группы Cell
    void signalReady();                                    // slotAdvance отработал
    void signalShowCellInfo();                             // показать окно информации о Cell
};

#endif // SCENE_H
