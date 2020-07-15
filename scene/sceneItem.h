#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QGraphicsItem>

class Scene;

class SceneItem : public QGraphicsItem
{
public:
    SceneItem(Scene *scene, QGraphicsItem *parent = nullptr);
    Scene *getScene() const;
    QPixmap* getPixmap();

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    Scene* m_Scene;
    QPixmap m_Pixmap;
    QRect m_Rect;
};

#endif // SCENEITEM_H
