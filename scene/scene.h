#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class SceneObject;
class SceneView;
class Field;
class Cell;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(SceneView* parent, Field* field);
    SceneObject* addObject(int x, int y);
    void fill();
    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);
    QGraphicsRectItem *borderRect() const;
    QHash<QPair<int, int>, SceneObject*>* objectList() const;
    QSize size() const;
    SceneObject *focusedObject() const;
    SceneView *getView() const;
    Field *getField() const;
       void StopAdvanse();

private:
    SceneView* m_View;
    Field* m_Field;
    QColor m_BackgroundColor;
    QGraphicsRectItem* m_BorderRect;
    QHash<QPair<int, int>, SceneObject*> m_ObjectList;
    QSize m_Size;
    qreal m_AverageDraw;                                    // среднее время отрисовки сцены
    bool m_StopAdvanse;                                     // остановка отрисовки перед выходом и .т.д

private Q_SLOTS:
    void slotAdvance(QSet<Cell*> cells);                    // перерисовать сцену

Q_SIGNALS:
    void signalProgress(int progress);                      // прогресс создания сцены
    void signalAdvansedTime(qint64 time);                   // сигнал статистики отрисовки
};

#endif // SCENE_H
