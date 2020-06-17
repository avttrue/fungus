#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class SceneObject;
class SceneView;
class Field;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(SceneView* parent, Field* field);
    SceneObject* addObject(int x, int y);
    void removeObject(SceneObject* object);
    void removeObject(int x, int y);
    void fill();
    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);
    QGraphicsRectItem *borderRect() const;
    QHash<QPair<int, int>, SceneObject*>* objectList() const;
    QSize size() const;
    SceneObject* focusedObject() const;
    SceneView *getView() const;

private:
    SceneView* m_View;
    Field* m_Field;
    QColor m_BackgroundColor;
    QGraphicsRectItem* m_BorderRect;
    QHash<QPair<int, int>, SceneObject*> m_ObjectList;
    QSize m_Size;

Q_SIGNALS:
    void signalProgress(int progress);
};

#endif // SCENE_H
