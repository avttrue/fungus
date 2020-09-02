#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QGraphicsItem>

class Scene;

class SceneItem : public QGraphicsItem
{
public:
    SceneItem(Scene *scene, QGraphicsItem *parent = nullptr);
    Scene *getScene() const;
    QPixmap* getBuffer();
    QPixmap* getPixmap();
    QRect rect() const;

protected:
    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    Scene* m_Scene;
    QPixmap m_Pixmap;
    QPixmap m_Buffer;
    QRect m_Rect;
};

#endif // SCENEITEM_H
