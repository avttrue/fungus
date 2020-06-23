#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QGraphicsItem>

class Cell;
class Scene;

class SceneItem : public QGraphicsItem
{
public:
    SceneItem(Scene *scene, QGraphicsItem *parent = nullptr);
    int size() const;
    QPoint index() const;
    void setIndex(const QPoint &value);
    Cell *getCell() const;
    void setCell(Cell *getCell);
    void setUpdate(bool value);
    Scene *getScene() const;
    QString getName() const;
    void setName(const QString &Name);

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void advance(int step) override;

private:
    Scene* m_Scene;
    Cell* m_Cell;
    int m_Size;
    QPoint m_Index;    
    bool m_Update;
    QString m_Name;
};

#endif // SCENEITEM_H
