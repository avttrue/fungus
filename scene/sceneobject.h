#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QGraphicsObject>

class Cell;
class Scene;

class SceneObject : public QGraphicsObject
{
    Q_OBJECT
public:
    SceneObject(Scene *scene, QGraphicsItem *parent = nullptr);
    int size() const;
    QPoint index() const;
    void setIndex(const QPoint &value);
    Cell *getCell() const;
    void setCell(Cell *getCell);
    void setUpdate(bool value);
    Scene *getScene() const;

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void advance(int step) override;

private:
    Scene* m_Scene;
    Cell* m_Cell;
    int m_Size;
    QPoint m_Index;    
    bool m_Update;
};

#endif // SCENEOBJECT_H
