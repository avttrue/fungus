#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Field;
class SceneView;
class SceneItem;
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
    void fillField();
    void setActionsEnable(bool value);
    void deleteField();
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
    QLabel* m_LabelFieldAvCalc;
    QLabel* m_LabelSceneAvDraw;
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
    void slotFieldAvCalc(qreal value);
    void slotAverageDraw(qreal value);
};
#endif // MAINWINDOW_H
