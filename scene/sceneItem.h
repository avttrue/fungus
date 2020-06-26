#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QGraphicsItem>

class Scene;

class SceneItem : public QGraphicsItem
{
public:
    SceneItem(Scene *scene, QGraphicsItem *parent = nullptr);
    Scene *getScene() const;
    QString getName() const;
    void setName(const QString &Name);
    void setPixmap(QPixmap pixmap);

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void advance(int step) override;

private:
    Scene* m_Scene;
    QPixmap m_Pixmap;
    QString m_Name;
};

#endif // SCENEITEM_H
