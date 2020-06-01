#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class SceneObject;
class Field;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent, Field* field);
    SceneObject* AddObject(int x, int y);
    void RemoveObject(SceneObject* object);
    void RemoveObject(int x, int y);
    void Fill();
    QColor BackgroundColor() const;
    void setBackgroundColor(const QColor &value);
    QGraphicsRectItem *BorderRect() const;
    QHash<QPair<int, int>, SceneObject*>* ObjectList();
    QSize Size() const;
    SceneObject* FocusedObject();

private:
    Field* m_Field;
    QColor m_BackgroundColor;
    QGraphicsRectItem* m_BorderRect;
    QHash<QPair<int, int>, SceneObject*> m_ObjectList;
    QSize m_Size;

protected:


Q_SIGNALS:
    void signalProgress(int progress);
    void signalObjectAdded(SceneObject* object);
    void signalObjectRemoved();
};

#endif // SCENE_H
