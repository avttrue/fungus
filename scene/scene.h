#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class SceneItem;
class SceneView;
class Field;
class Cell;

class Scene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(qreal AverageDraw READ getAverageDraw WRITE applyAverageDraw)

public:
    Scene(SceneView* parent, Field* field);
    void addSceneItem();
    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);
    QGraphicsRectItem *borderRect() const;
    QSize size() const;
    SceneView *getView() const;
    Field *getField() const;
    void StopAdvanse();
    SceneItem *getSceneItem() const;
    qreal getAverageDraw() const;
    void applyAverageDraw(qint64 time);

private:
    SceneView* m_View;
    Field* m_Field;
    SceneItem* m_SceneItem;
    QColor m_BackgroundColor;
    QGraphicsRectItem* m_BorderRect;
    QSize m_Size;
    qreal m_AverageDraw;                                   // среднее время отрисовки сцены
    bool m_StopAdvanse;                                    // остановка отрисовки перед выходом и .т.д

private Q_SLOTS:
    void slotAdvance(QVector<Cell*> cells);                   // перерисовать сцену

Q_SIGNALS:
    void signalProgress(int progress);                     // прогресс создания сцены
    void signalAverageDrawChangedUp(qreal AverageDraw);    // среднее время отрисовки сцены увеличилось
    void signalAverageDrawChangedDown(qreal AverageDraw);  // среднее время отрисовки сцены уменьшилось
};

#endif // SCENE_H
