#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

const QString MW_LABEL_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                               "stop: 0 transparent, "
                               "stop: 0.5 silver, "
                               "stop: 0.5 silver, "
                               "stop: 1 transparent); "
                               "padding: 0px 1px 0px 1px; border-radius: 3px;"
                               "color: black; font-family: monospace; font: bold;";
const QString MW_LABEL_STYLE_UP = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                  "stop: 0 transparent, "
                                  "stop: 0.5 silver, "
                                  "stop: 0.5 silver, "
                                  "stop: 1 transparent); "
                                  "padding: 0px 1px 0px 1px; border-radius: 3px;"
                                  "color: red; font-family: monospace; font: bold;";
const QString MW_LABEL_STYLE_DOWN = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                    "stop: 0 transparent, "
                                    "stop: 0.5 silver, "
                                    "stop: 0.5 silver, "
                                    "stop: 1 transparent); "
                                    "padding: 0px 1px 0px 1px; border-radius: 3px;"
                                    "color: green; font-family: monospace; font: bold;";
class Field;
class FieldRule;
class Cell;
class SceneView;
class QLabel;
class ClickableLabel;
class QProgressBar;
class QThread;
class JDocumentList;
class QToolButton;

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
    void createField(int w, int h, bool random = false);
    void fillField(bool random = false);
    void setMainActionsEnable(bool value);
    void setCellsActionsEnable(bool value);
    void deleteField();
    void stopThreadField();
    void setSceneFieldThreadPriority();
    void redrawScene();
    void CellsToJsonObject(QJsonObject *jobject, Cell* firstcell, Cell* secondcell, bool except_dead);
    QString CellsToJsonText(Cell *firstcell, Cell *secondcell, bool exceptdead);
    bool CellsFromJsonText(Cell *cell, const QString& text);
    bool CellsFromJsonObject(QJsonObject *jobject, Cell* cell);
    void stopFieldCalculating(); // корректная приостановка поля
    void RuleToJsonObject(FieldRule *rule, QJsonObject *jobject);
    bool RuleFromJsonObject(FieldRule *rule, QJsonObject *jobject);
    bool RuleFromJsonText(FieldRule *rule, const QString &text);
    bool RuleFromFilePath(FieldRule *rule, const QString &path);
    void saveRuleToFile(FieldRule *rule);
    void FieldToJsonObject(QJsonObject *jobject);
    bool FieldFromJsonObject(QJsonObject *jobject);
    void createSnapshot();
    void loadSnapshot(QJsonDocument* document);
    bool loadProjectFromJsonObject(QJsonObject *jobject);
    bool writeCompressData(const QByteArray& data, const QString &path);
    bool readUncompressData(QByteArray* data, const QString &path);

private:
    bool m_FieldRunning;   // состояние поля
    QThread* m_ThreadField;
    Field* m_Field;
    SceneView* m_SceneView;
    JDocumentList* m_Snapshots;
    QToolBar* m_TbMain;
    QToolBar* m_TbActions;
    QToolButton* m_BtnMenuRules;
    QAction* m_ActionNewProject;
    QAction* m_ActionZoomInScene;
    QAction* m_ActionZoomOutScene;
    QAction* m_ActionZoomUndoScene;
    QAction* m_ActionStepStop;
    QAction* m_ActionNewRule;
    QAction* m_ActionImportRule;
    QAction* m_ActionLoadEditRule;
    QAction* m_ActionInfoRule;
    QAction* m_ActionSaveProject;
    QAction* m_ActionLoadProject;
    QAction* m_ActionRun;
    QAction* m_ActionEditCell;
    QAction* m_ActionInfoCell;
    QAction* m_ActionInfoField;
    QAction* m_ActionSaveImageToFile;
    QAction* m_ActionSaveCellsToClipbord;
    QAction* m_ActionLoadCellsFromClipbord;
    QAction* m_ActionSaveCellsToPreset;
    QAction* m_ActionLoadCellsFromPreset;
    QAction* m_ActionClearCells;
    QAction* m_ActionRandomFill;
    QAction* m_ActionSelectAll;
    QAction* m_ActionCreateSnapshot;
    QAction* m_ActionSelectSnapshot;
    QLabel* m_LabelFieldZoom;
    QLabel* m_LabelFieldAge;
    QLabel* m_LabelFieldPause;
    QLabel* m_LabelFieldAvCalc;
    QLabel* m_LabelSceneAvDraw;
    ClickableLabel* m_LabelSelectedCell;
    QLabel* m_LabelRunningStatus;
    QProgressBar* m_ProgressBar;

private Q_SLOTS:
    void slotSetup();
    void slotSceneZoomIn();
    void slotSceneZoomOut();
    void slotZoomUndoScene();
    void slotNewProject();
    void slotStepStop();
    void slotRun();
    void slotFieldRunning(bool value);
    void slotFieldAge(uint value);
    void slotFieldAvCalc(qreal value);
    void slotFieldAvCalcUp(qreal value);
    void slotFieldAvCalcDown(qreal value);
    void slotAverageDrawUp(qreal value);
    void slotAverageDrawDown(qreal value);
    void slotSelectedCellChanged(Cell* cell);
    void slotSaveCellsToClipbord();
    void slotLoadCellsFromClipbord();
    void slotSaveCellsToPreset();
    void slotLoadCellsFromPreset();
    void slotClearCells();
    void slotInfoCell();
    void slotInfoField();
    void slotEditCell();
    void slotShowCell(Cell* cell);
    void slotSaveImageToFile();
    void slotRandomFill();
    void slotSelectAll();
    void slotSelectedCellsChanged(Cell* first, Cell* second);
    void slotLabelSelectedCellClick();
    void slotCreateSnapshot();
    void slotSelectSnapshot();
    void slotLoadProject();
    void slotSaveProject();
    void slotNewRule();
    void slotLoadEditRule();
    void slotInfoRule();
    void slotImportRule();

Q_SIGNALS:
    void signalStopField();
};

#endif // MAINWINDOW_H
