#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

const QString MW_LABEL_STYLE = "background-color: lightgray; color: black; font-family: monospace; font: bold;";
const QString MW_LABEL_STYLE_UP = "background-color: lightgray; color: red; font-family: monospace; font: bold;";
const QString MW_LABEL_STYLE_DOWN = "background-color: lightgray; color: green; font-family: monospace; font: bold;";

class Field;
class Cell;
class SceneView;
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
    QAction* m_ActionEditCell;
    QAction* m_ActionInfoCell;
    QAction* m_ActionShowSelectedCell;
    QLabel* m_LabelFieldZoom;
    QLabel* m_LabelFieldAge;
    QLabel* m_LabelFieldPause;
    QLabel* m_LabelFieldAvCalc;
    QLabel* m_LabelSceneAvDraw;
    QLabel* m_LabelFieldDeadCells;
    QLabel* m_LabelFieldAliveCells;
    QLabel* m_LabelFieldCursedCells;
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
    void slotFieldAvCalcUp(qreal value);
    void slotFieldAvCalcDown(qreal value);
    void slotAverageDrawUp(qreal value);
    void slotAverageDrawDown(qreal value);
    void slotFieldDeadCells(qint64 value);
    void slotFieldAliveCells(qint64 value);
    void slotFieldCursedCells(qint64 value);
    void slotSelectedCellChanged(Cell* cell);
    void slotInfoCell();
    void slotEditCell();
    void slotShowCell(Cell* cell);
    void slotShowSelectedCell();
};

#endif // MAINWINDOW_H
