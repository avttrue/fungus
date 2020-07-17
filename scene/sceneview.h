#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include<QGraphicsView>

class Scene;
class Field;
class Cell;
class GraphicsViewZoomer;

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    SceneView(QWidget *parent = nullptr);
    Scene* addScene(Field* filed);
    Scene* getScene() const;
    GraphicsViewZoomer *zoomer() const;
    void SetUpdateMode();

protected:
    bool eventFilter(QObject *object, QEvent *event);
    Cell* getCell(qreal x, qreal y);

private:
    Scene* m_Scene;
    GraphicsViewZoomer* m_Zoomer;
};

#endif // SCENEVIEW_H
