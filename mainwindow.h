#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Field;
class SceneView;
class SceneObject;
class QLabel;
class QProgressBar;
class QThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QProgressBar *ProgressBar() const;

protected:
    void closeEvent(QCloseEvent *event);
    void loadGui();
    void createScene();
    void createField(int w, int h);
    void setActionsEnable(bool value);
    void deleteObjects();
    void stopThreadField();

private:
    QThread* m_ThreadField;
    Field* m_Field;
    SceneView* m_SceneView;
    QAction* m_ActionZoomInScene;
    QAction* m_ActionZoomOutScene;
    QAction* m_ActionZoomUndoScene;
    QAction* m_ActionStepStop;
    QAction* m_ActionRun;
    QLabel* m_LabelFieldSize;
    QLabel* m_LabelFieldZoom;
    QLabel* m_LabelFocusedObject;
    QLabel* m_LabelFieldAge;
    QLabel* m_LabelFieldCalc;
    QLabel* m_LabelFieldAvCalc;
    QProgressBar* m_ProgressBar;

private Q_SLOTS:
    void slotSetup();
    void slotSceneZoomIn();
    void slotSceneZoomOut();
    void slotZoomUndoScene();
    void slotNewProject();
    void slotStepStop();
    void slotRun();
    void slotFieldAge(qint64 value);
    void slotFieldCalc(qint64 value);
    void slotFieldAvCalc(qreal value);
};
#endif // MAINWINDOW_H
