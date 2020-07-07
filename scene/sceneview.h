#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include<QGraphicsView>

const QStringList VIEW_UPDATE_MODE = {"SMART", "MINIMAL", "FULL"};

class Scene;
class Field;
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

private:
    Scene* m_Scene;
    GraphicsViewZoomer* m_Zoomer;

Q_SIGNALS:

};

#endif // SCENEVIEW_H
