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
    void fill();
    GraphicsViewZoomer *zoomer() const;
    void showCellInformationDialog(Cell* cell);
    void findObjectBySell(Cell* cell);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Scene* m_Scene;
    GraphicsViewZoomer* m_Zoomer;

Q_SIGNALS:

};

#endif // SCENEVIEW_H
