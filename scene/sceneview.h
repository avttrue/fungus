#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include<QGraphicsView>

class Scene;
class Field;
class GraphicsViewZoomer;

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    SceneView(QWidget *parent = nullptr);
    Scene* AddScene(Field* filed);
    Scene* getScene() const;
    void Fill();
    GraphicsViewZoomer *Zoomer() const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Scene* m_Scene;
    GraphicsViewZoomer* m_Zoomer;

Q_SIGNALS:

};

#endif // SCENEVIEW_H
