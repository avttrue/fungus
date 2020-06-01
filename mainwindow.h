#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Field;
class SceneView;
class SceneObject;
class QLabel;
class QProgressBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QProgressBar *ProgressBar() const;

protected:
    void closeEvent(QCloseEvent *event);
    void loadGui();
    void newProject();
    void createScene();
    void createField(int w, int h);

private:
    Field* m_Field;
    SceneView* m_SceneView;
    QAction* m_ActionZoomInScene;
    QAction* m_ActionZoomOutScene;
    QAction* m_ActionZoomUndoScene;
    QLabel* m_LabelFieldSize;
    QLabel* m_LabelFieldZoom;
    QLabel* m_LabelFocusedObject;
    QProgressBar* m_ProgressBar;

private Q_SLOTS:
    void slotSetup();
    void slotSceneZoomIn();
    void slotSceneZoomOut();
};
#endif // MAINWINDOW_H
