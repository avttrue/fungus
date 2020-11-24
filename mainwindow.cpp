#include "mainwindow.h"
#include "properties.h"
#include "controls/separators.h"
#include "controls/menucaption.h"
#include "controls/clickablelabel.h"
#include "controls/snapshotlist.h"
#include "helpers/helper.h"
#include "helpers/tooltipshortcut.h"
#include "helpers/widgethelper.h"
#include "scene/scene.h"
#include "scene/sceneitem.h"
#include "scene/sceneview.h"
#include "scene/graphicsviewzoomer.h"
#include "dialogs/dialogvalueslist.h"
#include "dialogs/dialogcellinformation.h"
#include "dialogs/dialogfieldinformation.h"
#include "dialogs/dialogeditrules.h"
#include "dialogs/dialoginfocontent.h"
#include "dialogs/dialogcellmonitor.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellinformation.h"
#include "field/fieldrule.h"
#include "field/fieldinformation.h"

#include <QtMath>
#include <QDebug>
#include <QApplication>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QClipboard>
#include <QMetaProperty>
#include <QRandomGenerator>
#include <QToolTip>
#include <QMenu>
#include <QToolButton>
#include <QSaveFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_FieldRunning(false),
      m_ThreadField(nullptr),
      m_Field(nullptr)
{
    setWindowIcon(QIcon(":/resources/img/mushroom.svg"));
    setWindowTitle(QString("%1 %2").arg(APP_NAME, APP_VERSION));
    m_Snapshots = new SnapshotList(this);
    loadGui();
    setWidgetToScreenCenter(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_SceneView->getScene()) m_SceneView->getScene()->StopAdvanse();

    deleteField();

    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height", height());
    settings.setValue("MainWindow/Width", width());

    config->deleteLater();
    event->accept();
}

void MainWindow::loadGui()
{
    // действия
    auto actionQt = new QAction(QIcon(":/resources/img/qt_logo.svg"), tr("About Qt"), this);
    QObject::connect(actionQt, &QAction::triggered, qApp, QApplication::aboutQt);
    actionQt->setAutoRepeat(false);

    auto actionAbout = new QAction(QIcon(":/resources/img/info.svg"), tr("About"), this);
    QObject::connect(actionAbout, &QAction::triggered, this, &MainWindow::slotAbout);
    actionAbout->setAutoRepeat(false);

    auto actionHelp = new QAction(QIcon(":/resources/img/question.svg"), tr("Help"), this);
    QObject::connect(actionHelp, &QAction::triggered, this, &MainWindow::slotHelp);
    actionHelp->setAutoRepeat(false);

    auto actionExit = new QAction(QIcon(":/resources/img/exit.svg"), tr("Exit"), this);
    QObject::connect(actionExit, &QAction::triggered, this, &MainWindow::close);
    actionExit->setAutoRepeat(false);

    m_ActionNewProject = new QAction(QIcon(":/resources/img/asterisk.svg"), tr("New project"), this);
    QObject::connect(m_ActionNewProject, &QAction::triggered, this, &MainWindow::slotNewProject);
    m_ActionNewProject->setShortcut(Qt::CTRL + Qt::Key_N);
    addShortcutToToolTip(m_ActionNewProject);
    m_ActionNewProject->setAutoRepeat(false);

    m_ActionTasks = new QAction(QIcon(":/resources/img/tasks.svg"), tr("Set project tasks"), this);
    QObject::connect(m_ActionTasks, &QAction::triggered, this, &MainWindow::slotTasks);
    m_ActionTasks->setEnabled(false);
    m_ActionTasks->setAutoRepeat(false);

    m_ActionLoadProject = new QAction(QIcon(":/resources/img/open_folder.svg"), tr("Load project"), this);
    QObject::connect(m_ActionLoadProject, &QAction::triggered, this, &MainWindow::slotLoadProject);
    m_ActionLoadProject->setShortcut(Qt::CTRL + Qt::Key_O);
    addShortcutToToolTip(m_ActionLoadProject);
    m_ActionLoadProject->setAutoRepeat(false);

    m_ActionSaveProject = new QAction(QIcon(":/resources/img/save.svg"), tr("Save project"), this);
    QObject::connect(m_ActionSaveProject, &QAction::triggered, this, &MainWindow::slotSaveProject);
    m_ActionSaveProject->setShortcut(Qt::CTRL + Qt::Key_S);
    addShortcutToToolTip(m_ActionSaveProject);
    m_ActionSaveProject->setAutoRepeat(false);
    m_ActionSaveProject->setEnabled(false);

    m_ActionNewRule = new QAction(QIcon(":/resources/img/asterisk.svg"), tr("New rule"), this);
    QObject::connect(m_ActionNewRule, &QAction::triggered, this, &MainWindow::slotNewRule);
    m_ActionNewRule->setAutoRepeat(false);

    m_ActionLoadEditRule = new QAction(QIcon(":/resources/img/open_folder.svg"), tr("Load and edit rule"), this);
    QObject::connect(m_ActionLoadEditRule, &QAction::triggered, this, &MainWindow::slotLoadEditRule);
    m_ActionLoadEditRule->setAutoRepeat(false);

    m_ActionImportRule = new QAction(QIcon(":/resources/img/check.svg"), tr("Edit current rule"), this);
    QObject::connect(m_ActionImportRule, &QAction::triggered, this, &MainWindow::slotImportRule);
    m_ActionImportRule->setShortcut(Qt::CTRL + Qt::Key_R);
    m_ActionImportRule->setAutoRepeat(false);
    m_ActionImportRule->setEnabled(false);

    m_ActionInfoRule = new QAction(QIcon(":/resources/img/info.svg"), tr("Current rule"), this);
    QObject::connect(m_ActionInfoRule, &QAction::triggered, this, &MainWindow::slotInfoRule);
    m_ActionInfoRule->setShortcut(Qt::ALT + Qt::Key_R);
    m_ActionInfoRule->setAutoRepeat(false);
    m_ActionInfoRule->setEnabled(false);

    m_ActionZoomInScene = new QAction(QIcon(":/resources/img/zoom_in.svg"), tr("Zoom IN"), this);
    QObject::connect(m_ActionZoomInScene, &QAction::triggered, this, &MainWindow::slotSceneZoomIn);
    m_ActionZoomInScene->setShortcut(Qt::CTRL + Qt::Key_Equal);
    addShortcutToToolTip(m_ActionZoomInScene);
    m_ActionZoomInScene->setEnabled(false);

    m_ActionZoomOutScene = new QAction(QIcon(":/resources/img/zoom_out.svg"), tr("Zoom OUT"), this);
    QObject::connect(m_ActionZoomOutScene, &QAction::triggered, this, &MainWindow::slotSceneZoomOut);
    m_ActionZoomOutScene->setShortcut(Qt::CTRL + Qt::Key_Minus);
    addShortcutToToolTip(m_ActionZoomOutScene);
    m_ActionZoomOutScene->setEnabled(false);

    m_ActionZoomUndoScene = new QAction(QIcon(":/resources/img/zoom_undo.svg"), tr("Zoom UNDO"), this);
    QObject::connect(m_ActionZoomUndoScene, &QAction::triggered, this, &MainWindow::slotZoomUndoScene);
    m_ActionZoomUndoScene->setShortcut(Qt::CTRL + Qt::Key_2);
    addShortcutToToolTip(m_ActionZoomUndoScene);
    m_ActionZoomUndoScene->setAutoRepeat(false);
    m_ActionZoomUndoScene->setEnabled(false);

    m_ActionZoomFit = new QAction(QIcon(":/resources/img/zoom_fit.svg"), tr("Fit to size"), this);
    QObject::connect(m_ActionZoomFit, &QAction::triggered, this, &MainWindow::slotSceneZoomFit);
    m_ActionZoomFit->setShortcut(Qt::CTRL + Qt::Key_1);
    addShortcutToToolTip(m_ActionZoomFit);
    m_ActionZoomFit->setAutoRepeat(false);
    m_ActionZoomFit->setEnabled(false);

    auto actionSetup = new QAction(QIcon(":/resources/img/setup.svg"), tr("Settings"), this);
    QObject::connect(actionSetup, &QAction::triggered, this, &MainWindow::slotSetup);
    actionSetup->setAutoRepeat(false);

    m_ActionStepStop = new QAction(QIcon(":/resources/img/step_stop.svg"), tr("Step / Stop"), this);
    QObject::connect(m_ActionStepStop, &QAction::triggered, this, &MainWindow::slotStepStop);
    m_ActionStepStop->setShortcut(Qt::CTRL + Qt::Key_Space);
    addShortcutToToolTip(m_ActionStepStop);
    m_ActionStepStop->setAutoRepeat(false);
    m_ActionStepStop->setEnabled(false);

    m_ActionRun = new QAction(QIcon(":/resources/img/run.svg"), tr("Run"), this);
    QObject::connect(m_ActionRun, &QAction::triggered, this, &MainWindow::slotRun);
    m_ActionRun->setAutoRepeat(false);
    m_ActionRun->setEnabled(false);

    m_ActionEditCell = new QAction(QIcon(":/resources/img/edit_cell.svg"), tr("Edit cell"), this);
    QObject::connect(m_ActionEditCell, &QAction::triggered, this, &MainWindow::slotEditCell);
    m_ActionEditCell->setShortcut(Qt::CTRL + Qt::Key_E);
    addShortcutToToolTip(m_ActionEditCell);
    m_ActionEditCell->setAutoRepeat(false);
    m_ActionEditCell->setEnabled(false);

    m_ActionInfoCell = new QAction(QIcon(":/resources/img/cell.svg"), tr("Cell information"), this);
    QObject::connect(m_ActionInfoCell, &QAction::triggered, this, &MainWindow::slotInfoCell);
    m_ActionInfoCell->setShortcut(Qt::CTRL + Qt::Key_I);
    addShortcutToToolTip(m_ActionInfoCell);
    m_ActionInfoCell->setAutoRepeat(false);
    m_ActionInfoCell->setEnabled(false);

    m_ActionSaveCellsToPreset = new QAction(QIcon(":/resources/img/preset_save.svg"), tr("Save selected cells as preset to file"), this);
    QObject::connect(m_ActionSaveCellsToPreset, &QAction::triggered, this, &MainWindow::slotSaveCellsToPreset);
    m_ActionSaveCellsToPreset->setEnabled(false);

    m_ActionLoadCellsFromPreset = new QAction(QIcon(":/resources/img/preset_load.svg"), tr("Load preset from file"), this);
    QObject::connect(m_ActionLoadCellsFromPreset, &QAction::triggered, this, &MainWindow::slotLoadCellsFromPreset);
    m_ActionLoadCellsFromPreset->setAutoRepeat(false);
    m_ActionLoadCellsFromPreset->setEnabled(false);

    m_ActionLoadFirstSnapshot = new QAction(this);
    QObject::connect(m_ActionLoadFirstSnapshot, &QAction::triggered, this, &MainWindow::slotLoadFirstSnapshot);
    m_ActionLoadFirstSnapshot->setShortcut(Qt::ALT + Qt::Key_W);
    addAction(m_ActionLoadFirstSnapshot);

    m_ActionInfoField = new QAction(QIcon(":/resources/img/field.svg"), tr("Field information"), this);
    QObject::connect(m_ActionInfoField, &QAction::triggered, this, &MainWindow::slotInfoField);
    m_ActionInfoField->setShortcut(Qt::CTRL + Qt::Key_F);
    addShortcutToToolTip(m_ActionInfoField);
    m_ActionInfoField->setAutoRepeat(false);
    m_ActionInfoField->setEnabled(false);

    m_ActionCellMonitor = new QAction(QIcon(":/resources/img/cell_monitor.svg"), tr("Cells monitor"), this);
    QObject::connect(m_ActionCellMonitor, &QAction::triggered, this, &MainWindow::slotCellMonitor);
    m_ActionCellMonitor->setAutoRepeat(false);
    m_ActionCellMonitor->setEnabled(false);

    m_ActionClearCells = new QAction(QIcon(":/resources/img/delete.svg"), tr("Clear cells"), this);
    QObject::connect(m_ActionClearCells, &QAction::triggered, this, &MainWindow::slotClearCells);
    m_ActionClearCells->setShortcut(Qt::CTRL + Qt::Key_D);
    addShortcutToToolTip(m_ActionClearCells);
    m_ActionClearCells->setAutoRepeat(false);
    m_ActionClearCells->setEnabled(false);

    m_ActionSaveCellsToClipbord = new QAction(QIcon(":/resources/img/copy.svg"), tr("Copy cells to clipboard"), this);
    QObject::connect(m_ActionSaveCellsToClipbord, &QAction::triggered, this, &MainWindow::slotSaveCellsToClipbord);
    m_ActionSaveCellsToClipbord->setShortcut(Qt::CTRL + Qt::Key_C);
    addShortcutToToolTip(m_ActionSaveCellsToClipbord);
    m_ActionSaveCellsToClipbord->setAutoRepeat(false);
    m_ActionSaveCellsToClipbord->setEnabled(false);

    m_ActionLoadCellsFromClipbord = new QAction(QIcon(":/resources/img/paste.svg"), tr("Paste cells from clipboard"), this);
    QObject::connect(m_ActionLoadCellsFromClipbord, &QAction::triggered, this, &MainWindow::slotLoadCellsFromClipbord);
    m_ActionLoadCellsFromClipbord->setShortcut(Qt::CTRL + Qt::Key_V);
    addShortcutToToolTip(m_ActionLoadCellsFromClipbord);
    m_ActionLoadCellsFromClipbord->setAutoRepeat(false);
    m_ActionLoadCellsFromClipbord->setEnabled(false);

    m_ActionCutCells = new QAction(QIcon(":/resources/img/cut.svg"), tr("Copy cells to clipboard and clear"), this);
    QObject::connect(m_ActionCutCells, &QAction::triggered, this, &MainWindow::slotCutCells);
    m_ActionCutCells->setShortcut(Qt::CTRL + Qt::Key_X);
    addShortcutToToolTip(m_ActionCutCells);
    m_ActionCutCells->setAutoRepeat(false);
    m_ActionCutCells->setEnabled(false);

    m_ActionRandomFill = new QAction(QIcon(":/resources/img/cells.svg"), tr("Random fill"), this);
    QObject::connect(m_ActionRandomFill, &QAction::triggered, this, &MainWindow::slotRandomFill);
    m_ActionRandomFill->setAutoRepeat(false);
    m_ActionRandomFill->setEnabled(false);

    m_ActionInvert = new QAction(QIcon(":/resources/img/sel_unsel.svg"), tr("Invert"), this);
    QObject::connect(m_ActionInvert, &QAction::triggered, this, &MainWindow::slotInvert);
    m_ActionInvert->setAutoRepeat(false);
    m_ActionInvert->setEnabled(false);

    m_ActionFlipHorizontal = new QAction(QIcon(":/resources/img/flip_h.svg"), tr("Flip on the horizontal"), this);
    QObject::connect(m_ActionFlipHorizontal, &QAction::triggered, this, &MainWindow::slotFlipHorizontal);
    m_ActionFlipHorizontal->setAutoRepeat(false);
    m_ActionFlipHorizontal->setEnabled(false);

    m_ActionFlipVertical = new QAction(QIcon(":/resources/img/flip_v.svg"), tr("Flip on the vertical"), this);
    QObject::connect(m_ActionFlipVertical, &QAction::triggered, this, &MainWindow::slotFlipVertical);
    m_ActionFlipVertical->setAutoRepeat(false);
    m_ActionFlipVertical->setEnabled(false);

    m_ActionSaveImageToFile = new QAction(QIcon(":/resources/img/camera.svg"), tr("Save image to file"), this);
    QObject::connect(m_ActionSaveImageToFile, &QAction::triggered, this, &MainWindow::slotSaveImageToFile);
    m_ActionSaveImageToFile->setAutoRepeat(false);
    m_ActionSaveImageToFile->setEnabled(false);

    m_ActionReport = new QAction(QIcon(":/resources/img/text.svg"), tr("Create report"), this);
    QObject::connect(m_ActionReport, &QAction::triggered, this, &MainWindow::slotReport);
    m_ActionReport->setAutoRepeat(false);
    m_ActionReport->setEnabled(false);

    m_ActionSelectAll = new QAction(QIcon(":/resources/img/select_all.svg"), tr("Select all cells"), this);
    QObject::connect(m_ActionSelectAll, &QAction::triggered, this, &MainWindow::slotSelectAll);
    m_ActionSelectAll->setAutoRepeat(false);
    m_ActionSelectAll->setShortcut(Qt::CTRL + Qt::Key_A);
    addShortcutToToolTip(m_ActionSelectAll);
    m_ActionSelectAll->setEnabled(false);

    m_ActionCreateSnapshot = new QAction(QIcon(":/resources/img/check.svg"), tr("Create snapshot"), this);
    QObject::connect(m_ActionCreateSnapshot, &QAction::triggered, this, &MainWindow::slotCreateSnapshot);
    m_ActionCreateSnapshot->setAutoRepeat(false);
    m_ActionCreateSnapshot->setShortcut(Qt::CTRL + Qt::Key_W);
    addShortcutToToolTip(m_ActionCreateSnapshot);
    m_ActionCreateSnapshot->setEnabled(false);

    m_ActionSelectSnapshot = new QAction(QIcon(":/resources/img/next_snapshot.svg"), tr("Load snapshot"), this);
    QObject::connect(m_ActionSelectSnapshot, &QAction::triggered, this, &MainWindow::slotSelectSnapshot);
    m_ActionSelectSnapshot->setAutoRepeat(false);
    m_ActionSelectSnapshot->setEnabled(false);

    // меню кнопки редактирования правил
    m_BtnMenuRules = new QToolButton(this);
    m_BtnMenuRules->setObjectName(CSS_TOOLBUTTON);
    m_BtnMenuRules->setToolTip(tr("Rules"));
    m_BtnMenuRules->setIcon(QIcon(":/resources/img/rule.svg"));
    m_BtnMenuRules->setPopupMode(QToolButton::InstantPopup);
    m_BtnMenuRules->setArrowType(Qt::NoArrow);
    auto menuRules = new QMenu(this);
    menuRules->addAction(new MenuCaption(menuRules, m_BtnMenuRules->toolTip()));
    menuRules->addAction(m_ActionInfoRule);
    menuRules->addSeparator();
    menuRules->addAction(m_ActionNewRule);
    menuRules->addAction(m_ActionLoadEditRule);
    menuRules->addAction(m_ActionImportRule);
    m_BtnMenuRules->setMenu(menuRules);

    // меню кнопки редактирования ячеек
    m_BtnMenuEditCells = new QToolButton(this);
    m_BtnMenuEditCells->setObjectName(CSS_TOOLBUTTON);
    m_BtnMenuEditCells->setToolTip(tr("Edit cells"));
    m_BtnMenuEditCells->setIcon(QIcon(":/resources/img/edit.svg"));
    m_BtnMenuEditCells->setPopupMode(QToolButton::InstantPopup);
    m_BtnMenuEditCells->setArrowType(Qt::NoArrow);
    auto menuEditCells = new QMenu(this);
    menuEditCells->addAction(new MenuCaption(menuRules, m_BtnMenuEditCells->toolTip()));
    menuEditCells->addAction(m_ActionClearCells);
    menuEditCells->addSeparator();
    menuEditCells->addAction(m_ActionRandomFill);
    menuEditCells->addAction(m_ActionInvert);
    menuEditCells->addAction(m_ActionFlipHorizontal);
    menuEditCells->addAction(m_ActionFlipVertical);
    m_BtnMenuEditCells->setMenu(menuEditCells);

    // тулбар основной
    m_TbMain = new QToolBar(this);
    m_TbMain->setMovable(false);
    m_TbMain->setOrientation(Qt::Horizontal);
    m_TbMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    addToolBarAction(m_TbMain, m_ActionNewProject, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionTasks, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionLoadProject, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionSaveProject, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionSaveImageToFile, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionReport, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionZoomFit, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionZoomUndoScene, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionZoomInScene, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionZoomOutScene, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionEditCell, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionInfoCell, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionCellMonitor, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionInfoField, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, m_ActionStepStop, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, m_ActionRun, CSS_TOOLBUTTON);
    m_TbMain->addWidget(new WidgetSpacer(this));
    addToolBarAction(m_TbMain, actionSetup, CSS_TOOLBUTTON);
    m_TbMain->addSeparator();
    addToolBarAction(m_TbMain, actionHelp, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, actionAbout, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, actionQt, CSS_TOOLBUTTON);
    addToolBarAction(m_TbMain, actionExit, CSS_TOOLBUTTON);
    addToolBar(Qt::TopToolBarArea, m_TbMain);

    // тулбар дополнительный
    m_TbActions = new QToolBar(this);
    m_TbActions->setMovable(false);
    m_TbActions->setOrientation(Qt::Vertical);
    m_TbActions->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    m_TbActions->addWidget(m_BtnMenuRules);
    m_TbActions->addSeparator();
    addToolBarAction(m_TbActions, m_ActionSaveCellsToPreset, CSS_TOOLBUTTON);
    addToolBarAction(m_TbActions, m_ActionLoadCellsFromPreset, CSS_TOOLBUTTON);
    m_TbActions->addSeparator();
    addToolBarAction(m_TbActions, m_ActionCreateSnapshot, CSS_TOOLBUTTON);
    addToolBarAction(m_TbActions, m_ActionSelectSnapshot, CSS_TOOLBUTTON);
    m_TbActions->addSeparator();
    addToolBarAction(m_TbActions, m_ActionSelectAll, CSS_TOOLBUTTON);
    m_TbActions->addSeparator();
    addToolBarAction(m_TbActions, m_ActionCutCells, CSS_TOOLBUTTON);
    addToolBarAction(m_TbActions, m_ActionSaveCellsToClipbord, CSS_TOOLBUTTON);
    addToolBarAction(m_TbActions, m_ActionLoadCellsFromClipbord, CSS_TOOLBUTTON);
    m_TbActions->addWidget(m_BtnMenuEditCells);
    addToolBar(Qt::LeftToolBarArea, m_TbActions);

    // SceneView
    m_SceneView = new SceneView(this);
    setCentralWidget(m_SceneView);

    // статусбар
    auto statusBar = new QStatusBar(this);

    m_LabelRunningStatus = new QLabel(this);
    m_LabelRunningStatus->setPixmap(QPixmap(":/resources/img/stop.svg").
                                    scaled(QFontMetrics(font()).height(), QFontMetrics(font()).height(),
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    statusBar->addWidget(m_LabelRunningStatus);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Zoom:"), this));
    m_LabelFieldZoom = new QLabel("1.00", this);
    QObject::connect(m_SceneView->zoomer(), &GraphicsViewZoomer::signalZoomed,
                     [=](qreal value){ m_LabelFieldZoom->setText(QString::number(value, 'f', 2)); });
    statusBar->addWidget(m_LabelFieldZoom);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Snapshots:"), this));
    m_LabelSnapshotsCount = new QLabel("0", this);
    QObject::connect(m_Snapshots, &SnapshotList::signalCountChanged,
                     [=](int value){ m_LabelSnapshotsCount->setText(QString::number(value)); });
    statusBar->addWidget(m_LabelSnapshotsCount);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Age:"), this));
    m_LabelFieldAge = new QLabel("-", this);
    m_LabelFieldAge->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldAge);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Pause:"), this));
    m_LabelFieldPause = new QLabel(tr("%1 ms").arg(QString::number(config->SceneCalculatingMinPause())), this);
    statusBar->addWidget(m_LabelFieldPause);

    statusBar->addWidget(new QLabel(tr("Calc:"), this));
    m_LabelFieldAvCalc = new QLabel("-", this);
    m_LabelFieldAvCalc->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldAvCalc);

    statusBar->addWidget(new QLabel(tr("Draw:"), this));
    m_LabelSceneAvDraw = new QLabel("-", this);
    m_LabelSceneAvDraw->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelSceneAvDraw);

    statusBar->addWidget(new SeparatorV(this));

    m_LabelSelectedCell = new ClickableLabel("-", this);
    m_LabelSelectedCell->setToolTip(tr("Click to show cell"));
    QObject::connect(m_LabelSelectedCell, &ClickableLabel::signalClicked, this, &MainWindow::slotLabelSelectedCellClick);
    statusBar->addWidget(m_LabelSelectedCell);
    statusBar->addWidget(new SeparatorV(this));

    m_ProgressBar = new QProgressBar(this);
    m_ProgressBar->setAlignment(Qt::AlignLeft);
    m_ProgressBar->setFixedWidth(6 * config->ButtonSize());
    m_ProgressBar->setRange(0, 0);
    m_ProgressBar->setVisible(false);
    statusBar->addPermanentWidget(m_ProgressBar);

    setStatusBar(statusBar);

    // финал
    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    resize(settings.value("MainWindow/Width", WINDOW_WIDTH).toInt(),
           settings.value("MainWindow/Height", WINDOW_HEIGHT).toInt());
}

void MainWindow::slotStepStop()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    if(!m_Field->isCalculating())
    {
        if(config->FieldFirstSnapshot() && !m_Snapshots->count())
            writeSnapshot();
        m_SceneView->getScene()->clearMultiSelection();
        m_Field->setRuleOn(true);
        m_Field->setCalculatingNonstop(false);
        m_Field->slotStartCalculating();
        m_ThreadField->start();
        setSceneFieldThreadPriority();
        return;
    }

    stopFieldCalculating();
}

void MainWindow::slotRun()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    if(!m_Field->isCalculating())
    {
        if(config->FieldFirstSnapshot() && !m_Field->getInformation()->getAge())
            writeSnapshot();
        m_SceneView->getScene()->clearMultiSelection();
        m_Field->setRuleOn(true);
        m_Field->setCalculatingNonstop(true);
        m_Field->slotStartCalculating();
        m_ThreadField->start();
        setSceneFieldThreadPriority();
    }
}

void MainWindow::slotFieldRunning(bool value)
{
    m_FieldRunning = value;
    if(value)
        m_LabelRunningStatus->setPixmap(QPixmap(":/resources/img/running.svg").
                                        scaled(QFontMetrics(font()).height(), QFontMetrics(font()).height(),
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else
        m_LabelRunningStatus->setPixmap(QPixmap(":/resources/img/paused.svg").
                                        scaled(QFontMetrics(font()).height(), QFontMetrics(font()).height(),
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::createScene()
{
    qDebug() << __func__;
    auto scene = m_SceneView->addScene(m_Field);
    scene->addSceneItem();

    QObject::connect(scene, &Scene::signalAverageDrawChangedUp, this, &MainWindow::slotAverageDrawUp);
    QObject::connect(scene, &Scene::signalAverageDrawChangedDown, this, &MainWindow::slotAverageDrawDown);
    QObject::connect(scene, &Scene::signalSelectedCellChanged, this, &MainWindow::slotSelectedCellChanged);
    QObject::connect(scene, &Scene::signalShowCellInfo, this, &MainWindow::slotInfoCell);
    QObject::connect(scene, &Scene::signalSelectedCellsChanged, this, &MainWindow::slotSelectedCellsChanged);
}

void MainWindow::createField(int w, int h, int random)
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(scene) scene->StopAdvanse();

    deleteField();

    m_ThreadField = new QThread;
    QObject::connect(m_ThreadField, &QObject::destroyed, [=](){ qDebug() << "ThreadField destroyed"; });

    m_Field = new Field(w, h);

    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAgeChanged, this, &MainWindow::slotFieldAge);
    QObject::connect(m_Field, &Field::signalCalculating, this, &MainWindow::slotFieldRunning);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChanged, this, &MainWindow::slotFieldAvCalc);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedUp, this, &MainWindow::slotFieldAvCalcUp);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedDown, this, &MainWindow::slotFieldAvCalcDown);
    QObject::connect(this, &MainWindow::signalStopField, m_Field, &Field::slotStopCalculating, Qt::DirectConnection);

    fillField(random);
    m_Field->moveToThread(m_ThreadField);  // NOTE: field выполняется не в основном потоке

    QObject::connect(m_Field, &Field::signalCalculatingDone, this, &MainWindow::stopThreadField, Qt::DirectConnection);
    QObject::connect(m_ThreadField, &QThread::started, m_Field, &Field::calculate, Qt::DirectConnection);
}

void MainWindow::fillField(int random)
{
    qDebug() << __func__ << "random:" << random;
    m_ProgressBar->setRange(0, m_Field->height());
    m_ProgressBar->setValue(0);
    m_ProgressBar->show();

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=](int step)
    {
        if(step < m_Field->height()) m_ProgressBar->setValue(step);
        else
        {
            m_ProgressBar->hide();
            qDebug() << "Field filling disconnection:" << QObject::disconnect(*conn);
        }
    };
    *conn = QObject::connect(m_Field, &Field::signalFillingProgress, func);

    m_Field->fill(random);
}

void MainWindow::setMainActionsEnable(bool value)
{
    auto enable = m_SceneView->getScene() ? value : false;

    m_ActionNewProject->setEnabled(value);
    m_ActionLoadProject->setEnabled(value);
    m_BtnMenuRules->setEnabled(value);

    m_ActionInfoRule->setEnabled(enable);
    m_ActionImportRule->setEnabled(enable);
    m_ActionSaveProject->setEnabled(enable);
    m_ActionTasks->setEnabled(enable);
    m_ActionZoomInScene->setEnabled(enable);
    m_ActionZoomOutScene->setEnabled(enable);
    m_ActionZoomUndoScene->setEnabled(enable);
    m_ActionZoomFit->setEnabled(enable);
    m_ActionStepStop->setEnabled(enable);
    m_ActionInfoField->setEnabled(enable);
    m_ActionCellMonitor->setEnabled(enable);
    m_ActionSaveImageToFile->setEnabled(enable);
    m_ActionReport->setEnabled(enable);
    m_ActionRun->setEnabled(enable);
    m_ActionSelectAll->setEnabled(enable);
    m_ActionCreateSnapshot->setEnabled(enable);
    m_ActionSelectSnapshot->setEnabled(enable);
}

void MainWindow::setCellsActionsEnable(bool value)
{
    auto scene = m_SceneView->getScene();
    bool enable = scene ? value : false;
    bool group_enable = (enable &&
                         scene->getSelectedCell() &&
                         scene->getSecondSelectedCell() &&
                         scene->getSelectedCell() != scene->getSecondSelectedCell())
            ? enable : false;
    bool single_enable = (enable && scene->getSelectedCell()) ? enable : false;

    m_ActionLoadCellsFromPreset->setEnabled(enable);
    m_ActionLoadCellsFromClipbord->setEnabled(enable);

    m_ActionEditCell->setEnabled(single_enable);
    m_ActionInfoCell->setEnabled(single_enable);

    m_ActionSaveCellsToClipbord->setEnabled(group_enable);
    m_ActionSaveCellsToPreset->setEnabled(group_enable);
    m_ActionClearCells->setEnabled(group_enable);
    m_ActionCutCells->setEnabled(group_enable);
    m_ActionRandomFill->setEnabled(group_enable);
    m_ActionInvert->setEnabled(group_enable);
    m_ActionFlipHorizontal->setEnabled(group_enable);
    m_ActionFlipVertical->setEnabled(group_enable);
}

void MainWindow::deleteField()
{
    qDebug() << __func__;
    Q_EMIT signalStopField(); // здесь сразу выключаем поле

    if(m_Field)
    {
        m_Field->AbortCalculating();

        qDebug() << "MainWindow waits for field stopping...";
        while(m_ThreadField->isRunning())
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        delete m_Field;
        m_Field = nullptr;
    }

    if(m_ThreadField)
    {
        delete m_ThreadField;
        m_ThreadField = nullptr;
    }
}

void MainWindow::stopThreadField()
{
    qDebug() << __func__;
    m_ThreadField->quit();
    m_ThreadField->requestInterruption();
}

void MainWindow::setSceneFieldThreadPriority()
{
    if(!m_ThreadField || !m_ThreadField->isRunning()) return;

    auto thread_priority = QThread::NormalPriority;
    auto mode = config->SceneFieldThreadPriority().toUpper();

    if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(0)) thread_priority = QThread::LowPriority;
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(1)) thread_priority = QThread::NormalPriority;
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(2)) thread_priority = QThread::HighPriority;
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(3)) thread_priority = QThread::HighestPriority;
    else qCritical() << "Wrong settins value 'Scene/FieldThreadPriority'" <<  mode;

    m_ThreadField->setPriority(thread_priority);
    qDebug() << "Field thread priority:" << m_ThreadField->priority() << mode;
}

bool MainWindow::getJsonRootObject(const QByteArray &data, QJsonObject* root)
{
    qDebug() << __func__;
    QJsonParseError p_error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &p_error);

    if(doc.isNull() || doc.isEmpty())
    {
        qCritical() << "QJsonDocument is empty";
        return false;
    }

    if(p_error.error != QJsonParseError::NoError)
    {
        qCritical() << "JsonParseError:" << p_error.errorString();
        return false;
    }

    *root = doc.object();
    if(root->isEmpty())
    {
        qCritical() << "Root JsonObject is empty";
        return false;
    }
    return true;
}

bool MainWindow::checkJsonDocumentVersion(QJsonObject *root)
{
    qDebug() << __func__;

    auto app = root->value("Application").toString();
    auto vers = root->value("Version").toString();
    qDebug() << "Data version:" << app << vers;

    if(app != APP_NAME)
    {
        qCritical() << "Incorrect value: 'Application' =" << app;
        QMessageBox::critical(this, tr("Error"),
                              tr("Data error. \n Incorrect application name: '%1'").arg(app));
        return false;
    }

    if(config->JsonIgnoreDataVersion()) return true;

    if(vers != FORMAT_VERSION)
    {
        qCritical() << "Incorrect value: 'Version' =" << vers;
        QMessageBox::critical(this, tr("Error"),
                              tr("Data error. \n Incorrect data format version: '%1'").arg(vers));
        return false;
    }
    return true;
}

void MainWindow::CellsToJsonObject(QJsonObject* jobject, Cell *firstcell, Cell *secondcell, bool except_dead)
{
    qDebug() << __func__;
    if(!firstcell)
    {
        qCritical() << "First cell is null";
        return;
    }

    if(!secondcell)
    {
        qCritical() << "Second cell is null";
        return;
    }

    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return;
    }

    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());

    m_ProgressBar->setRange(0, (xmax - xmin) * (ymax - ymin));
    m_ProgressBar->setValue(0);
    m_ProgressBar->show();

    QJsonArray cells;
    for(int x = xmin; x <= xmax; x++)
    {
        for(int y = ymin; y <= ymax; y++)
        {
            auto c = m_Field->getCell({x, y});
            auto ci = c->getNewInfo();

            if(except_dead && ci->getState() == Kernel::CellState::DEAD) continue;

            auto ci_mo = ci->metaObject();

            QJsonObject obj_prop;
            for(int i = ci_mo->propertyOffset(); i < ci_mo->propertyCount(); ++i)
            {
                auto p = ci_mo->property(i);

                auto value = ci->property(p.name());
                QJsonValue jvalue;
                if(value.userType() == qMetaTypeId<Kernel::CellState>()) jvalue = value.toInt();
                else jvalue = value.toJsonValue();

                obj_prop.insert(p.name(), jvalue);
            }
            cells.append(QJsonObject({{"Index", QJsonObject({{"X", x - xmin}, {"Y", y - ymin}})},
                                      {"Properties", obj_prop}}));

            m_ProgressBar->setValue(cells.count());
            QApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
        }
    }

    QJsonObject obj_size{{"Height", ymax - ymin + 1}, {"Width", xmax - xmin + 1}};
    jobject->insert("Size", obj_size);
    jobject->insert("Cells", cells);

    qDebug() << "Copied to JsonObject" << cells.count() << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    m_ProgressBar->hide();
}

QString MainWindow::CellsToJsonText(Cell *firstcell, Cell *secondcell, bool exceptdead)
{
    qDebug() << __func__;
    if(!firstcell)
    {
        qCritical() << "First cell is null";
        return "";
    }

    if(!secondcell)
    {
        qCritical() << "Second cell is null";
        return "";
    }

    auto datetime = QDateTime::currentDateTime().toString(config->DateTimeFormat());
    QJsonDocument document;
    QJsonObject obj_root {{"DateTime", datetime},
                          {"Application", APP_NAME},
                          {"Version", FORMAT_VERSION}};

    CellsToJsonObject(&obj_root, firstcell, secondcell, exceptdead);
    document.setObject(obj_root);

    auto json_mode = config->JsonCompactMode() ? QJsonDocument::Compact : QJsonDocument::Indented;
    return document.toJson(json_mode);
}

bool MainWindow::CellsFromJsonText(Cell *cell, const QString &text)
{
    qDebug() << __func__;

    if(!validateScene()) return false;
    auto scene = m_SceneView->getScene();

    QJsonObject root_object;
    if(!getJsonRootObject(text.toUtf8(), &root_object)) return false;
    if(!checkJsonDocumentVersion(&root_object)) return false;

    auto obj_size = root_object.value("Size").toObject();
    int w = obj_size.value("Width").toInt();
    int h = obj_size.value("Height").toInt();
    qDebug() << "Field size:" << h << "X" << w;

    if(scene->getField()->width() < w || scene->getField()->height() < h)
    { QMessageBox::warning(this, tr("Warning"),
                           tr("Pasted field size (%1X%2) exceeds the allowed size (%3X%4).").
                           arg(QString::number(scene->getField()->width()),
                               QString::number(scene->getField()->height()),
                               QString::number(w), QString::number(h))); return false; }

    int x, y = 0;
    if(!cell)
    {
        x = qFloor(static_cast<qreal>(scene->getField()->width() - w) / 2);
        y = qFloor(static_cast<qreal>(scene->getField()->height() - h) / 2);

        auto answer = QMessageBox::question(this, tr("Question"), tr("The target cell is not selected."
                                                                     "\n To try placing it in the center of the field?"
                                                                     "\n New target cell: [%1 X %2]").
                                            arg(QString::number(x), QString::number(y)));

        if (answer == QMessageBox::No) return false;

        cell = m_Field->getCell({x, y});
        scene->selectCell(cell, false);
    }
    else
    {
        x = cell->getIndex().x();
        y = cell->getIndex().y();
    }

    if(x + w > scene->getField()->width() ||
            y + h > scene->getField()->height())
    { QMessageBox::warning(this, tr("Warning"),
                           tr("Pasted field (%1X%2) does not fit in the selected coordinates (%3X%4).").
                           arg(QString::number(w), QString::number(h),
                               QString::number(x), QString::number(y))); return false; }


    if(!CellsFromJsonObject(&root_object, cell))
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at inserting cells."));
        return false;
    }

    return true;
}

bool MainWindow::CellsFromJsonObject(QJsonObject *jobject, Cell *cell)
{
    qDebug() << __func__;
    if(!cell)
    {
        qCritical() << "Cell is null";
        return false;
    }

    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return false;
    }

    auto time = QDateTime::currentMSecsSinceEpoch();
    auto cx = cell->getIndex().x();
    auto cy = cell->getIndex().y();
    auto obj_cells = jobject->value("Cells").toArray();

    if(obj_cells.isEmpty()) { qCritical() << "JsonArray 'Cells' is empty"; return false; }

    m_ProgressBar->setRange(0, obj_cells.count());
    m_ProgressBar->setValue(0);
    m_ProgressBar->show();

    for(auto o: obj_cells)
    {
        auto obj_index = o.toObject().value("Index").toObject();
        if(obj_index.isEmpty()) { qCritical() << "JsonObject 'Index' is empty"; return false; }

        auto obj_x = obj_index.value("X");
        if(obj_x.isUndefined()) { qCritical() << "JsonValue 'Index.X' is undefined"; return false; }

        auto obj_y = obj_index.value("Y");
        if(obj_y.isUndefined()) { qCritical() << "JsonValue 'Index.Y' is undefined"; return false; }

        auto obj_prop = o.toObject().value("Properties").toObject();
        if(obj_prop.isEmpty()) { qCritical() << "JsonObject 'Properties' is empty"; return false; }

        auto x = obj_index.value("X").toInt();
        auto y = obj_index.value("Y").toInt();
        auto c = m_Field->getCell({cx + x, cy + y});
        auto ni = c->getNewInfo();

        for(auto key : obj_prop.keys())
        {
            auto v = obj_prop.value(key).toVariant();
            ni->setProperty(key.toLatin1(), v.toUInt());
        }
        c->applyInfo();

        m_ProgressBar->setValue(m_ProgressBar->value() + 1);
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
    }

    qDebug() << "Pasted from JsonObject" << obj_cells.count() << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    m_ProgressBar->hide();
    return true;
}

void MainWindow::stopFieldCalculating()
{
    qDebug() << __func__;
    if(!m_SceneView->getScene()) return;
    if(!m_Field) return;

    if(m_Field->isCalculating())
    {
        m_ProgressBar->setRange(0, 0);
        m_ProgressBar->setValue(0);
        m_ProgressBar->show();
        Q_EMIT signalStopField();

        qDebug() << "MainWindow waits for field stopping...";
        while(m_FieldRunning)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        qDebug() << "MainWindow waits for scene ready...";
        while(m_Field->isWaitScene())
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        m_ProgressBar->hide();
    }
}

void MainWindow::RuleToJsonObject(FieldRule* rule, QJsonObject *jobject)
{
    qDebug() << __func__;
    if(!rule)
    {
        qCritical() << "FieldRule is null";
        return;
    }

    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return;
    }

    QJsonObject obj_rule;
    QJsonArray obj_acts;
    auto fr_mo = rule->metaObject();
    for(int i = fr_mo->propertyOffset(); i < fr_mo->propertyCount(); ++i)
    {
        auto p = fr_mo->property(i);
        auto value = rule->property(p.name());
        if(QString(p.name()) != "Activity")
            obj_rule.insert(p.name(), value.toJsonValue());
    }
    for(auto a: rule->getActivity())
    {
        /* {Type, SelfState, Target, TargetState, Operand, Operator, OperandValue, Break} */
        QJsonObject obj_act;
        obj_act.insert("Type", a.at(0).toInt());
        obj_act.insert("SelfState", a.at(1).toInt());
        obj_act.insert("Target", a.at(2).toInt());
        obj_act.insert("TargetState", a.at(3).toInt());
        obj_act.insert("Operand", a.at(4).toInt());
        obj_act.insert("Operator", a.at(5).toInt());
        obj_act.insert("OperandValue", a.at(6).toInt());
        obj_act.insert("Break", a.at(7).toBool());
        obj_acts.append(obj_act);
    }
    obj_rule.insert("Name", rule->objectName());
    obj_rule.insert("Activity", obj_acts);
    jobject->insert("Rule", obj_rule);
}

bool MainWindow::RuleFromJsonObject(FieldRule *rule, QJsonObject *jobject)
{
    qDebug() << __func__;
    if(!rule)
    {
        qCritical() << "FieldRule is null";
        return false;
    }

    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return false;
    }

    auto obj_rule = jobject->value("Rule").toObject();
    if(obj_rule.isEmpty()) { qCritical() << "JsonObject 'Rule' is empty"; return false; }

    auto obj_act = obj_rule.value("Activity").toArray();
    if(obj_act.isEmpty()) { qCritical() << "JsonArray 'Activity' is empty"; return false; }

    if(!obj_rule.contains("Name"))
    { qCritical() << "JsonValue 'Name' is absent"; return false; }
    rule->setObjectName(obj_rule["Name"].toString());

    if(obj_rule.contains("Description"))
        rule->setDescription(obj_rule["Description"].toString());
    else qCritical() << "JsonValue 'Description' is absent";

    if(obj_rule.contains("CurseTime"))
        rule->setCurseTime(obj_rule["CurseTime"].toInt());
    else qCritical() << "JsonValue 'CurseTime' is absent";

    Activity activity;
    /* {Type, SelfState, Target, TargetState, Operand, Operator, OperandValue} */
    for(auto a : obj_act)
    {
        auto o = a.toObject();
        QVector<QVariant> v;

        if(!o.contains("Type"))
        { qCritical() << "JsonValue 'Activity.Type' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::ActivityType>(o["Type"].toInt())));

        if(!o.contains("SelfState"))
        { qCritical() << "JsonValue 'Activity.SelfState' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::CellState>(o["SelfState"].toInt())));

        if(!o.contains("Target"))
        { qCritical() << "JsonValue 'Activity.Target' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::ActivityTarget>(o["Target"].toInt())));

        if(!o.contains("TargetState"))
        { qCritical() << "JsonValue 'Activity.TargetState' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::CellState>(o["TargetState"].toInt())));

        if(!o.contains("Operand"))
        { qCritical() << "JsonValue 'Activity.Operand' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::ActivityOperand>(o["Operand"].toInt())));

        if(!o.contains("Operator"))
        { qCritical() << "JsonValue 'Activity.Operator' is absent"; return false; }
        v.append(QVariant::fromValue(static_cast<Kernel::ActivityOperator>(o["Operator"].toInt())));

        if(!o.contains("OperandValue"))
        { qCritical() << "JsonValue 'Activity.OperandValue' is absent"; return false; }
        v.append(o["OperandValue"].toInt());

        if(!o.contains("Break"))
        {  qCritical() << "JsonValue 'Activity.Break' is absent"; v.append(true); }
        else v.append(o["Break"].toBool());

        activity.append(v);
    }
    rule->setActivity(activity);
    return true;
}

bool MainWindow::RuleFromJsonText(FieldRule* rule, const QString& text)
{
    qDebug() << __func__;

    QJsonObject root_object;
    if(!getJsonRootObject(text.toUtf8(), &root_object)) return false;
    if(!checkJsonDocumentVersion(&root_object)) return false;

    return RuleFromJsonObject(rule, &root_object);
}

bool MainWindow::RuleFromFilePath(FieldRule *rule, const QString &path)
{
    bool ok;
    auto text = fileToText(path, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at loading data from file: '%1'").arg(path));
        return false;
    }

    if(!RuleFromJsonText(rule, text))
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at reading rule from file: '%1'").arg(path));
        return false;
    }
    return true;
}

void MainWindow::saveRule(FieldRule *rule)
{
    qDebug() << __func__;
    auto fileext = RULE_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save rule"), config->PathRulesDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto dot_fileext = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileext, Qt::CaseInsensitive)) filename.append(dot_fileext);

    saveRuleToFile(rule, filename);
}

bool MainWindow::saveRuleToFile(FieldRule *rule, const QString &path)
{
    qDebug() << __func__;
    auto datetime = QDateTime::currentDateTime().toString(config->DateTimeFormat());
    QJsonDocument document;
    QJsonObject obj_root {{"DateTime", datetime},
                          {"Application", APP_NAME},
                          {"Version", FORMAT_VERSION}};
    RuleToJsonObject(rule, &obj_root);
    document.setObject(obj_root);

    auto json_mode = config->JsonCompactMode() ? QJsonDocument::Compact : QJsonDocument::Indented;
    auto text = document.toJson(json_mode);
    if(!textToFile(text, path))
    {
        qCritical() << "Data writing error:" << path;
        QMessageBox::critical(this, tr("Error"), tr("Data writing error. \n File: '%1'").arg(path));
        return false;
    }
    return true;
}

void MainWindow::FieldToJsonObject(QJsonObject *jobject)
{
    qDebug() << __func__;
    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return;
    }

    auto fi = m_Field->getInformation();
    auto fi_mo = fi->metaObject();

    QJsonObject obj_prop;
    for(int i = fi_mo->propertyOffset(); i < fi_mo->propertyCount(); ++i)
    {
        auto p = fi_mo->property(i);
        auto value = fi->property(p.name());
        obj_prop.insert(p.name(), value.toJsonValue());
    }
    QJsonObject obj_field;
    obj_field.insert("Properties", obj_prop);
    jobject->insert("Field", obj_field);
}

bool MainWindow::FieldFromJsonObject(QJsonObject *jobject)
{
    qDebug() << __func__;
    if(!jobject)
    {
        qCritical() << "JsonObject is null";
        return false;
    }
    auto obj_field = jobject->value("Field").toObject();
    if(obj_field.isEmpty()) { qCritical() << "JsonObject 'Field' is empty"; return false; }

    auto obj_prop = obj_field.value("Properties").toObject();
    if(obj_prop.isEmpty()) { qCritical() << "JsonObject 'Properties' is empty"; return false; }

    auto cell = m_Field->getCell({0, 0});
    if(!CellsFromJsonObject(jobject, cell)) return false;

    auto fi = m_Field->getInformation();
    for(auto key : obj_prop.keys())
    {
        auto v = obj_prop.value(key).toVariant();
        if(v.type() == QVariant::Double)
            fi->setProperty(key.toLatin1(), v.toDouble());
        else
            fi->setProperty(key.toLatin1(), v.toUInt());
    }
    return true;
}

void MainWindow::createSnapshot()
{
    qDebug() << __func__;

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);
    writeSnapshot();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::writeSnapshot()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    if(m_Field->getInformation()->getAge() == 0 &&
            config->FieldFirstSnapshotClearList())
        m_Snapshots->clear();

    auto name = QString::number(m_Field->getInformation()->getAge());
    auto datetime = QDateTime::currentDateTime().toString(config->DateTimeFormat());

    QJsonDocument document;
    QJsonObject obj_root {{"DateTime", datetime},
                          {"Application", APP_NAME},
                          {"Version", FORMAT_VERSION}};

    FieldToJsonObject(&obj_root);

    auto firstcell = m_Field->getCell({0, 0});
    auto secondcell = m_Field->getCell({m_Field->width() - 1, m_Field->height() - 1});
    CellsToJsonObject(&obj_root, firstcell, secondcell, false);

    document.setObject(obj_root);
    auto pixmap = m_SceneView->getScene()->getSceneItem()->getPixmap();
    m_Snapshots->addItem(name, {document, QPixmap(*pixmap)});
}

void MainWindow::loadSnapshot(QJsonDocument* document)
{
    qDebug() << __func__;

    if(!validateScene()) return;

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto root_object = document->object();
    if(FieldFromJsonObject(&root_object)) m_Field->updateScene();

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

bool MainWindow::loadProjectFromJsonObject(QJsonObject *jobject)
{
    qDebug() << __func__;
    auto obj_size = jobject->value("Size").toObject();
    if(obj_size.isEmpty())
    {
        qCritical() << "JsonObject 'Size' is empty";
        return false;
    }

    auto w = obj_size.value("Width").toInt();
    auto h = obj_size.value("Height").toInt();
    if(w <= 0 && h <= 0)
    {
        qCritical() << "Incorrect values Size (w, h):" << w << h;
        return false;
    }

    createField(w, h, false);
    if(!FieldFromJsonObject(jobject)) return false;

    auto rule = new FieldRule;
    if(!RuleFromJsonObject(rule, jobject))
    {
        rule->deleteLater();
        return false;
    }
    m_Field->setRule(rule);
    return true;
}

bool MainWindow::writeCompressData(const QByteArray &data, const QString &path)
{
    qDebug() << __func__ << "compression level:" << config->ProjectFileCompressionLevel();
    auto time = QDateTime::currentMSecsSinceEpoch();
    QByteArray ba = qCompress(data, config->ProjectFileCompressionLevel());
    QSaveFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(ba);
    qDebug() << "Compress & write data:" << data.size() << "bytes in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    return file.commit();
}

bool MainWindow::readUncompressData(QByteArray *data, const QString &path)
{
    qDebug() << __func__;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "File could not be opened:" << path;
        return false;
    }

    auto time = QDateTime::currentMSecsSinceEpoch();
    QByteArray ba = file.readAll();
    file.close();
    if(!ba.size())
    {
        qCritical() << "File is empty:" << path;
        return false;
    }

    *data = qUncompress(ba);
    qDebug() << "Read & uncompress data:" << ba.size() << "bytes in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    return data->size();
}

void MainWindow::slotSetup()
{
    qDebug() << __func__;
    stopFieldCalculating();
    const QVector<QString> keys = {tr("00#_Common options"),
                                   tr("01#_Date and time format"),
                                   tr("02#_Write logs to file (restart required)"),
                                   tr("03#_Buttons size"),
                                   tr("04#_Rewrite resources at start"),
                                   tr("05#_Compress data in project files"),
                                   tr("06#_Open reports at saving"),
                                   tr("07#_Open images at saving"),
                                   tr("08#_Scene options"),
                                   tr("09#_Antialiasing"),
                                   tr("10#_Update mode"),
                                   tr("11#_Background color"),
                                   tr("12#_Selection color"),
                                   tr("13#_Zoom factor"),
                                   tr("14#_Cells options"),
                                   tr("15#_Alive cell color"),
                                   tr("16#_Trait cell color"),
                                   tr("17#_Dead cell color"),
                                   tr("18#_Cursed cell color"),
                                   tr("19#_Indicate value alive cell age"),
                                   tr("20#__Brightness"),
                                   tr("21#__Maximal factor"),
                                   tr("22#__Age diapason"),
                                   tr("23#_Indicate value cursed cell age"),
                                   tr("24#__Brightness"),
                                   tr("25#__Maximal factor"),
                                   tr("26#__Age diapason"),
                                   tr("27#_Field options"),
                                   tr("28#_Create first snapshot always"),
                                   tr("29#_Clear snapshot list at first snapshot"),
                                   tr("30#_Minimum pause at calculating (ms)"),
                                   tr("31#_Copy to clipboard except dead cells"),
                                   tr("32#_Save to preset except dead cells"),
                                   tr("33#_Field thread priority"),
                                  };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
     {keys.at(2), {QVariant::Bool, config->WriteLogsToFile()}},
     {keys.at(3), {QVariant::Int, config->ButtonSize(), 16, 100}},
     {keys.at(4), {QVariant::Bool, config->RewriteResource()}},
     {keys.at(5), {QVariant::Bool, config->ProjectFileCompression()}},
     {keys.at(6), {QVariant::Bool, config->ReportAutoopen()}},
     {keys.at(7), {QVariant::Bool, config->ImageAutoopen()}},
     {keys.at(8), {}},
     {keys.at(9), {QVariant::Bool, config->SceneViewAntialiasing()}},
     {keys.at(10), {QVariant::StringList, config->SceneViewUpdateMode(), 0, SCENE_VIEW_UPDATE_MODES, DialogValueMode::OneFromList}},
     {keys.at(11), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(12), {QVariant::String, config->SceneSelectColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(13), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
     {keys.at(14), {}},
     {keys.at(15), {QVariant::String, config->SceneCellAliveColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(16), {QVariant::String, config->SceneCellTraitColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(17), {QVariant::String, config->SceneCellDeadColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(18), {QVariant::String, config->SceneCellCurseColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(19), {QVariant::Bool, config->CellAliveAgeIndicate()}},
     {keys.at(20), {QVariant::StringList, config->CellAliveAgeIndicBright(), 0, SCENE_CELL_BRIGHTNESS_VALUES, DialogValueMode::OneFromList}},
     {keys.at(21), {QVariant::Int, config->CellAliveAgeIndicFactor(), 150, 1000}},
     {keys.at(22), {QVariant::Int, config->CellAliveAgeIndicDiapason(), 2, 50}},
     {keys.at(23), {QVariant::Bool, config->CellCurseAgeIndicate()}},
     {keys.at(24), {QVariant::StringList, config->CellCurseAgeIndicBright(), 0, SCENE_CELL_BRIGHTNESS_VALUES, DialogValueMode::OneFromList}},
     {keys.at(25), {QVariant::Int, config->CellCurseAgeIndicFactor(), 150, 1000}},
     {keys.at(26), {QVariant::Int, config->CellCurseAgeIndicDiapason(), 2, 50}},
     {keys.at(27), {}},
     {keys.at(28), {QVariant::Bool, config->FieldFirstSnapshot()}},
     {keys.at(29), {QVariant::Bool, config->FieldFirstSnapshotClearList()}},
     {keys.at(30), {QVariant::Int, config->SceneCalculatingMinPause(), 0, 10000}},
     {keys.at(31), {QVariant::Bool, config->CopyToClipboardExceptDead()}},
     {keys.at(32), {QVariant::Bool, config->SaveToPresetExceptDead()}},
     {keys.at(33), {QVariant::StringList, config->SceneFieldThreadPriority(), 0, SCENE_FIELD_THREAD_PRIORITIES, DialogValueMode::OneFromList}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/setup.svg", tr("Settings"), &map);
    dvl->resize(config->SetupWindowWidth(), config->SetupWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setSetupWindowWidth(size.width());
        config->setSetupWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    // common 0
    config->setDateTimeFormat(map.value(keys.at(1)).value.toString());
    config->setWriteLogsToFile(map.value(keys.at(2)).value.toBool());
    config->setButtonSize(map.value(keys.at(3)).value.toInt());
    config->setRewriteResource(map.value(keys.at(4)).value.toInt());
    config->setProjectFileCompression(map.value(keys.at(5)).value.toBool());
    config->setReportAutoopen(map.value(keys.at(6)).value.toBool());
    config->setImageAutoopen(map.value(keys.at(7)).value.toBool());
    // scene 8
    config->setSceneViewAntialiasing(map.value(keys.at(9)).value.toBool());
    config->setSceneViewUpdateMode(map.value(keys.at(10)).value.toString());
    config->setSceneBgColor(map.value(keys.at(11)).value.toString());
    config->setSceneSelectColor(map.value(keys.at(12)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(13)).value.toDouble());
    // cell 14
    config->setSceneCellAliveColor(map.value(keys.at(15)).value.toString());
    config->setSceneCellTraitColor(map.value(keys.at(16)).value.toString());
    config->setSceneCellDeadColor(map.value(keys.at(17)).value.toString());
    config->setSceneCellCurseColor(map.value(keys.at(18)).value.toString());
    config->setCellAliveAgeIndicate(map.value(keys.at(19)).value.toBool());
    config->setCellAliveAgeIndicBright(map.value(keys.at(20)).value.toString());
    config->setCellAliveAgeIndicFactor(map.value(keys.at(21)).value.toInt());
    config->setCellAliveAgeIndicDiapason(map.value(keys.at(22)).value.toInt());
    config->setCellCurseAgeIndicate(map.value(keys.at(23)).value.toBool());
    config->setCellCurseAgeIndicBright(map.value(keys.at(24)).value.toString());
    config->setCellCurseAgeIndicFactor(map.value(keys.at(25)).value.toInt());
    config->setCellCurseAgeIndicDiapason(map.value(keys.at(26)).value.toInt());
    // field 27
    config->setFieldFirstSnapshot(map.value(keys.at(28)).value.toBool());
    config->setFieldFirstSnapshotClearList(map.value(keys.at(29)).value.toBool());
    config->setSceneCalculatingMinPause(map.value(keys.at(30)).value.toInt());
    config->setCopyToClipboardExceptDead(map.value(keys.at(31)).value.toBool());
    config->setSaveToPresetExceptDead(map.value(keys.at(32)).value.toBool());
    config->setSceneFieldThreadPriority(map.value(keys.at(33)).value.toString());

    // применение настроек
    m_TbMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    m_TbActions->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    m_LabelFieldPause->setText(tr("%1 ms").arg(QString::number(config->SceneCalculatingMinPause())));
    setSceneFieldThreadPriority();
    m_SceneView->setUpdateMode();
    m_SceneView->setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    if(m_SceneView->getScene())
    {
        m_SceneView->getScene()->applyCellIndication();
        m_SceneView->getScene()->applyCellsColors();
        m_SceneView->getScene()->setBackgroundColor(config->SceneBgColor());
        m_SceneView->getScene()->setSelectionMarkColor(config->SceneSelectColor());
        m_SceneView->update();
        m_SceneView->getScene()->getField()->updateScene();
        m_SceneView->zoomer()->setKeyModifier(config->SceneZoomModifier());
        m_SceneView->zoomer()->setZoomFactorBase(config->SceneScaleStep());
    }
}

void MainWindow::slotEditCell()
{
    qDebug() << __func__;
    stopFieldCalculating();

    if(!validateSelectedCell()) return;

    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    auto multyselection =  !secondcell ? false : true;
    auto cni = firstcell->getNewInfo();

    auto statelist = listKernelEnum("CellState");
    statelist.removeAll("ANY");
    statelist.removeAll("NOT_CURSED");
    statelist.removeAll("NOT_ALIVE");

    QVector<QString> keys =
    { tr("00#_Cell properties"),
      tr("01#_State"),
      tr("02#_Trait (for AlIVE cells)"),
      tr("03#_Cell statistics"),
      tr("04#_Age"),
      tr("05#_Generation")};
    if(multyselection) keys.append(
                { tr("06#_Group operations"),
                  tr("07#_Apply to all"),
                  tr("08#_Only statistics")});

    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList,
                    getNameKernelEnum("CellState", static_cast<int>(cni->getState())), 0,
                    statelist, DialogValueMode::OneFromList}},

      {keys.at(2), {QVariant::Bool, cni->isTrait()}},
      {keys.at(3), {}},
      {keys.at(4), {QVariant::Int, cni->getAge(), 0, 0}},
      {keys.at(5), {QVariant::Int, cni->getGeneration(), 0, 0}}
    };
    if(multyselection)
    {
        QMap<QString, DialogValue> addmap =
        { {keys.at(6), {}},
          {keys.at(7), {QVariant::Bool, true}},
          {keys.at(8), {QVariant::Bool, false}}};
        map.insert(addmap);
    }

    auto dvl = new DialogValuesList(this, ":/resources/img/point.svg",
                                    tr("Edit cell %1").arg(firstcell->objectName()), &map);
    dvl->resize(config->EditCellWindowWidth(), config->EditCellWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setEditCellWindowWidth(size.width());
        config->setEditCellWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    if(multyselection && map.value(keys.at(7)).value.toBool())
    {
        auto only_stat = map.value(keys.at(8)).value.toBool();
        auto time = QDateTime::currentMSecsSinceEpoch();
        auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
        auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
        auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
        auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
        auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

        for(int x = xmin; x <= xmax; x++)
        {
            for(int y = ymin; y <= ymax; y++)
            {
                auto c = m_Field->getCell({x, y});
                cni = c->getNewInfo();
                if(!only_stat)
                {
                    auto state = statelist.indexOf(map.value(keys.at(1)).value.toString());
                    cni->setState(static_cast<Kernel::CellState>(state));
                    cni->setTrait(map.value(keys.at(2)).value.toBool());
                }
                cni->setAge(map.value(keys.at(4)).value.toUInt());
                cni->setGeneration(map.value(keys.at(5)).value.toUInt());
                c->applyInfo();
            }
        }
        qDebug() << "Group of" << count << "cells editing complete in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    }
    else
    {
        auto state = statelist.indexOf(map.value(keys.at(1)).value.toString());
        cni->setState(static_cast<Kernel::CellState>(state));
        cni->setTrait(map.value(keys.at(2)).value.toBool());
        cni->setAge(map.value(keys.at(4)).value.toUInt());
        cni->setGeneration(map.value(keys.at(5)).value.toUInt());
        firstcell->applyInfo();
    }
    m_Field->updateScene();
}

void MainWindow::slotNewProject()
{
    qDebug() << __func__;
    stopFieldCalculating();

    QMultiMap<QString, FieldRule*> ruleslist;

    if(config->SceneCreateDefaultRule()) // default rule
    {
        auto rule = new FieldRule;
        rule->setDefault();
        ruleslist.insert(tr("%1 (default)").arg(rule->objectName()), rule);
    }

    // rule files
    QDir dir(config->PathRulesDir());
    dir.setNameFilters(QStringList(QString("*.%1").arg(RULE_FILE_EXTENSION)));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    auto rulecount = dir.entryList().count();

    m_ProgressBar->setRange(0, rulecount);
    m_ProgressBar->setValue(0);
    m_ProgressBar->show();
    qDebug() << "Rule files count:" << rulecount;
    for(auto f: dir.entryList())
    {
        m_ProgressBar->setValue(m_ProgressBar->value() + 1);
        auto rule = new FieldRule;
        if(!RuleFromFilePath(rule, config->PathRulesDir() + QDir::separator() + f))
            continue;
        ruleslist.insert(rule->objectName(), rule);
    }
    m_ProgressBar->hide();
    qDebug() << "Rules loaded:" << ruleslist.count();

    int last_rule = 0;
    for(int i = 0; i < ruleslist.count(); i++)
    {
        if(ruleslist.keys().at(i) == config->SceneLastRule())
        {
            last_rule = i;
            break;
        }
    }

    const QVector<QString> keys = {
        tr("00#_Field properties"),
        tr("01#_Size (w X h)"),
        tr("02#_Rule [%1]:").arg(QString::number(ruleslist.count())),
        tr("03#_View"),
        tr("04#_Cell size"),
        tr("05#_Grid lines width"),
        tr("06#_Options"),
        tr("07#_Random fill"),
        tr("08#__Density of random fill, %"),
        tr("09#_Show field information"),
    };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::Int, config->SceneFieldSize(), 2, 10000}},
     {keys.at(2), {QVariant::StringList, ruleslist.keys().at(last_rule), 0, QStringList(ruleslist.keys()), DialogValueMode::OneFromList}},
     {keys.at(3), {}},
     {keys.at(4), {QVariant::Int, config->SceneCellSize(), 1, 100}},
     {keys.at(5), {QVariant::Int, config->SceneGridLineWidth(), 0, 100}},
     {keys.at(6), {}},
     {keys.at(7), {QVariant::Bool, false}},
     {keys.at(8), {QVariant::Int, config->FieldRandomisationValue(), 1, 100}},
     {keys.at(9), {QVariant::Bool, config->WindowShowFieldInfo()}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/asterisk.svg", tr("New project"), &map);
    dvl->resize(config->ProjectWindowWidth(), config->ProjectWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setProjectWindowWidth(size.width());
        config->setProjectWindowHeight(size.height());
    });
    if(!dvl->exec())
    {
        for(auto r: ruleslist) r->deleteLater();
        return;
    }
    // properties
    config->setSceneFieldSize(map.value(keys.at(1)).value.toInt());
    auto currentrule = map.value(keys.at(2)).value.toString();
    config->setSceneLastRule(currentrule);
    qDebug() << "Selected rule:" << currentrule;
    // view
    config->setSceneCellSize(map.value(keys.at(4)).value.toInt());
    config->setSceneGridLineWidth(map.value(keys.at(5)).value.toInt());
    // options
    auto rnd_on = map.value(keys.at(7)).value.toBool();
    config->setFieldRandomisationValue(map.value(keys.at(8)).value.toInt());
    config->setWindowShowFieldInfo(map.value(keys.at(9)).value.toBool());

    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto random = rnd_on ? config->FieldRandomisationValue() : 0;
    createField(config->SceneFieldSize(), config->SceneFieldSize(), random);

    m_Field->setRule(ruleslist.value(currentrule));
    for(auto r: ruleslist) if(!r->parent()) r->deleteLater();

    m_Snapshots->clear();
    createScene();
    if(random) m_Field->updateScene();

    setWindowTitle(QString("%1 %2 <%3> [%4 X %5 X %6]").
                   arg(APP_NAME, APP_VERSION, currentrule,
                       QString::number(m_Field->width()),
                       QString::number(m_Field->height()),
                       QString::number(config->SceneCellSize())));

    setMainActionsEnable(true);
    setCellsActionsEnable(true);

    m_SceneView->zoomer()->resetZoom();
    m_LabelFieldAvCalc->setText("0 ms");
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

    if(config->WindowShowFieldInfo()) showInfoField(false);
    config->setUnsavedTasksEnabled(false);
}

bool MainWindow::validateScene()
{
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        qCritical() << "Scene not created";
        return false;
    }
    return true;
}

bool MainWindow::validateSelectedCell()
{
    if(!validateScene()) return false;

    auto scene = m_SceneView->getScene();
    auto cell = scene->getSelectedCell();

    if(!cell)
    {
        qCritical() << "Target cell is empty";
        return false;
    }
    return true;
}

bool MainWindow::validateSelectedCells()
{
    if(!validateScene()) return false;

    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        qCritical() << "Target cells not selected";
        return false;
    }

    return true;
}

bool MainWindow::savePixmapToFile(QPixmap *pixmap, const QString &file)
{
    qDebug() << __func__;
    auto imgext = config->ImageFileFormat();
    auto success_saving = pixmap->save(file, imgext.toUpper().toLatin1().constData());
    if(!success_saving)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at file saving. Path: '%1'").arg(file));
        qCritical() << "Error at file saving: " << file;
    }
    return success_saving;
}

void MainWindow::slotSceneZoomIn()
{
    auto factor = config->SceneScaleStep() + 100 * (config->SceneScaleStep() - 1);
    m_SceneView->zoomer()->Zoom(factor, true);
}

void MainWindow::slotSceneZoomOut()
{
    auto factor = 1 / (config->SceneScaleStep() + 100 * (config->SceneScaleStep() - 1));
    m_SceneView->zoomer()->Zoom(factor, true);
}

void MainWindow::slotZoomUndoScene()
{ m_SceneView->zoomer()->resetZoom(); }

void MainWindow::slotSceneZoomFit()
{ m_SceneView->zoomer()->ZoomFitToView(); }

void MainWindow::slotSaveCellsToClipbord()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    cellsToClipboard(firstcell, secondcell);

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotLoadCellsFromClipbord()
{
    qDebug() << __func__;

    if(!validateScene()) return;
    auto scene = m_SceneView->getScene();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto clipboard = QGuiApplication::clipboard();
    auto text = clipboard->text();

    if(CellsFromJsonText(scene->getSelectedCell(), text)) m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotSaveCellsToPreset()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    auto fileext = PRESET_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save preset"), config->PathPresetsDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto dot_fileext = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileext, Qt::CaseInsensitive)) filename.append(dot_fileext);

    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto text = CellsToJsonText(firstcell, secondcell, config->SaveToPresetExceptDead());
    if(!textToFile(text, filename))
        QMessageBox::critical(this, tr("Error"), tr("Data writing error. \n File: '%1'").arg(filename));

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotLoadCellsFromPreset()
{
    qDebug() << __func__;

    if(!validateScene()) return;
    auto scene = m_SceneView->getScene();

    stopFieldCalculating();

    auto fileext = PRESET_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getOpenFileName(this, tr("Load preset"), config->PathPresetsDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    bool ok;
    auto text = fileToText(filename, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at loading data from file: '%1'").arg(filename));
        return;
    }

    setMainActionsEnable(false);
    setCellsActionsEnable(false);
    if(CellsFromJsonText(scene->getSelectedCell(), text)) m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotClearCells()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    clearCells(firstcell, secondcell);

    m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotCutCells()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    cellsToClipboard(firstcell, secondcell);

    clearCells(firstcell, secondcell);

    m_Field->updateScene();

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotInfoCell()
{
    auto cell = m_SceneView->getScene()->getSelectedCell();
    if(!cell) { m_ActionInfoCell->setDisabled(true); return; }

    if(DialogCellInformation::FindPreviousCopy(cell)) return;

    auto dci = new DialogCellInformation(this, cell);
    dci->resize(config->CellInfoWindowWidth(), config->CellInfoWindowHeight());
    QObject::connect(dci, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setCellInfoWindowWidth(size.width());
        config->setCellInfoWindowHeight(size.height());
    });
    QObject::connect(dci, &DialogCellInformation::signalShowCell, this, &MainWindow::slotShowCell);
    dci->show();
}

void MainWindow::showInfoField(bool unique)
{
    if(!m_Field) { m_ActionInfoField->setDisabled(true); return; }

    auto title = tr("Field");

    if(unique && findPreviousWindowCopy(title)) return;

    auto dfi = new DialogFieldInformation(this, title, m_Field);
    dfi->resize(config->FieldInfoWindowWidth(), config->FieldInfoWindowHeight());
    QObject::connect(dfi, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setFieldInfoWindowWidth(size.width());
        config->setFieldInfoWindowHeight(size.height());
    });
    dfi->show();
}

void MainWindow::clearCells(Cell* firstcell, Cell* secondcell)
{
    qDebug() << __func__;
    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

    for(int x = xmin; x <= xmax; x++)
    {
        for(int y = ymin; y <= ymax; y++)
        {
            auto c = m_Field->getCell({x, y});
            c->clear();
        }
    }
    qDebug() << "Cleared" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

void MainWindow::cellsToClipboard(Cell *firstcell, Cell *secondcell)
{
    auto text = CellsToJsonText(firstcell, secondcell, config->CopyToClipboardExceptDead());
    auto clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

void MainWindow::slotInfoField() { showInfoField(); }

void MainWindow::slotShowCell(Cell *cell)
{
    qDebug() << __func__;

    if(!validateScene()) return;
    auto scene = m_SceneView->getScene();

    if(!cell)
    {
        qDebug() << "Target cell is empty";
        return;
    }

    scene->selectCell(cell, false);
    m_SceneView->centerOn(cell->getIndex() * config->SceneCellSize());
}

void MainWindow::slotSaveImageToFile()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    auto fileext = config->ImageFileFormat().toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save image"), config->PathReportsDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto dot_fileformat = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileformat, Qt::CaseInsensitive)) filename.append(dot_fileformat);

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto pixmap = m_SceneView->getScene()->getSceneItem()->getPixmap();
    auto success_saving = savePixmapToFile(pixmap, filename);

    if(config->ImageAutoopen() && success_saving)
    {
        if(!OpenUrl(filename))
            QMessageBox::critical(this, tr("Error"), tr("Error at open URL:\n'%1'").arg(filename));
    }

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotReport()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    const QVector<QString> keys = {
        tr("00#_Report options"),
        tr("01#_Field properties"),
        tr("02#_Field rule"),
        tr("03#_Field statistics"),
        tr("04#_Field image"),
        tr("05#_Snapshots data"),
        tr("06#_Rule file"),
        tr("07#_Project file"),
    };
    QMap<QString, DialogValue> map = {
        {keys.at(0), {}},
        {keys.at(1), {QVariant::Bool, true}},
        {keys.at(2), {QVariant::Bool, true}},
        {keys.at(3), {QVariant::Bool, true}},
        {keys.at(4), {QVariant::Bool, true}},
        {keys.at(5), {QVariant::Bool, true}},
        {keys.at(6), {QVariant::Bool, true}},
        {keys.at(7), {QVariant::Bool, true}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/text.svg", tr("Report"), &map);
    dvl->resize(config->ReportWindowWidth(), config->ReportWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setReportWindowWidth(size.width());
        config->setReportWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    QString filename; // файл отчёта
    QString dirname;  // каталог доп.файлов отчёта

    auto fileext = config->ReportFileFormat().toLower();
    filename = QFileDialog::getSaveFileName(this, tr("Save report"), config->PathReportsDir(),
                                            tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto dot_fileformat = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileformat, Qt::CaseInsensitive)) filename.append(dot_fileformat);

    QFileInfo fileinfo(filename);
    dirname = fileinfo.dir().absolutePath() + QDir::separator() +
            fileinfo.fileName().remove(dot_fileformat).replace(' ', '_') + "_files";

    QDir dir(dirname);
    if(dir.exists(dirname) && !dir.removeRecursively())
    {
        qCritical() << "Directory exists and not be deleted:" << dirname;
        QMessageBox::critical(this, tr("Error"), tr("Directory exists and not be deleted:\n'%1'").arg(dirname));
        return;
    }

    if(!CreateDir(dirname))
    {
        QMessageBox::critical(this, tr("Error"), tr("Directory cannot be created:\n'%1'").arg(dirname));
        return;
    }

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    QString caption(tr("Report"));
    QString statcaption(tr("Statistics"));
    QString imgcaption(tr("Image"));
    QString propcaption(tr("Properties"));

    QString reportcontent;
    reportcontent.append(QString("<h2>%1</h2>\n").arg(caption));

    if(map.value(keys.at(1)).value.toBool()) // properties
    {
        reportcontent.append("<hr><div class = 'CONTENTDIV'>\n");

        reportcontent.append(QString("<h2>%1</h2><ul>\n").arg(propcaption));
        reportcontent.append(QString("<li>%1:&nbsp;%2&nbsp;X&nbsp;%3&nbsp;X&nbsp;%4</li>\n").
                             arg(tr("Size (Widtn X Height X Cell size)"),
                                 QString::number(m_Field->width()),
                                 QString::number(m_Field->height()),
                                 QString::number(config->SceneCellSize())));

        reportcontent.append(QString("<li>%1:&nbsp;%2</li>\n").
                             arg(tr("Cells count"),
                                 m_Field->getInformation()->property("CellsCount").toString()));

        reportcontent.append("</ul></div>\n");
    }

    if(map.value(keys.at(2)).value.toBool()) // rule
    {
        reportcontent.append("<hr><div class = 'CONTENTDIV'>\n");
        reportcontent.append(QString("<h2>%1</h2>\n").arg(tr("Rule")));
        reportcontent.append(QString("<h3>%1</h3><ul>\n").arg(propcaption));
        for(auto s: m_Field->getRule()->PropertiesToString().split('\n'))
            reportcontent.append(QString("<li>%1</li>\n").arg(s));
        reportcontent.append("</ul>");

        reportcontent.append(QString("<h3>%1</h3><ul>\n").arg(tr("Activities")));
        for(auto a: m_Field->getRule()->getActivity())
            reportcontent.append(QString("<li>%1</li>\n").arg(ActivityElementToString(a).replace(' ', "&nbsp;")));
        reportcontent.append("</ul>\n");
        reportcontent.append("</div>\n");
    }

    if(map.value(keys.at(5)).value.toBool()) // Snapshots data
    {
        reportcontent.append("<hr><div class = 'CONTENTDIV'>\n");
        reportcontent.append(QString("<h2>%1</h2>\n").arg(tr("Snapshots")));

        for(auto key: m_Snapshots->keys())
        {
            if(map.value(keys.at(3)).value.toBool()) // statistics
            {
                reportcontent.append(QString("<h3>%1</h3>\n").arg(statcaption));

                auto jdoc = m_Snapshots->getSnapshot(key).document;
                auto root_object = jdoc.object();

                auto obj_field = root_object.value("Field").toObject();
                if(obj_field.isEmpty()) { qCritical() << "JsonObject 'Field' is empty"; continue; }

                auto obj_prop = obj_field.value("Properties").toObject();
                if(obj_prop.isEmpty()) { qCritical() << "JsonObject 'Properties' is empty"; continue; }

                reportcontent.append("<ul>\n");
                for(auto propkey : obj_prop.keys())
                {
                    auto v = obj_prop.value(propkey).toVariant();
                    reportcontent.append(QString("<li>%1:&nbsp;%2</li>\n").arg(propkey, v.toString()));
                }
                reportcontent.append("</ul>\n");
            }

            if(map.value(keys.at(4)).value.toBool()) // image
            {
                reportcontent.append(QString("<h3>%1</h3>\n").arg(imgcaption));
                auto imgname = key + "." + config->ImageFileFormat().toLower();
                auto imgfile = dirname + QDir::separator() + imgname;
                auto imgbasefile =  QString(".") + QDir::separator() +
                        QFileInfo(dirname).baseName() + QDir::separator() + imgname;
                auto pixmap = m_Snapshots->getSnapshot(key).pixmap;

                if(QFile(imgfile).exists() || savePixmapToFile(&pixmap, imgfile))
                {
                    // проверка на exists - для совместимости картинок снапшотов и текущего поля
                    reportcontent.append(QString("<img src = '%1' alt = '%2'>\n").
                                         arg(imgbasefile, imgname));
                }
            }

            reportcontent.append("<div class = 'SEPARATORDIV'></div>\n");
        }
        reportcontent.append("</div>\n");
    }

    // current statistics and image
    if(map.value(keys.at(3)).value.toBool() || map.value(keys.at(4)).value.toBool())
    {
        reportcontent.append("<hr><div class = 'CONTENTDIV'>\n");
        reportcontent.append(QString("<h2>%1</h2>\n").arg(tr("Current state")));
    }

    if(map.value(keys.at(3)).value.toBool()) // statistics
    {
        reportcontent.append(QString("<h3>%1</h3>\n").arg(statcaption));
        reportcontent.append("<ul>\n");

        auto fi = m_Field->getInformation();
        auto fi_mo = fi->metaObject();
        QStringList sl;
        for(int i = fi_mo->propertyOffset(); i < fi_mo->propertyCount(); ++i)
        {
            auto p = fi_mo->property(i);
            auto value = fi->property(p.name());
            sl.append(QString("<li>%1:&nbsp;%2</li>\n").arg(p.name(), value.toString()));
        }
        sl.sort();
        for(auto s: sl) reportcontent.append(s);
        reportcontent.append("</ul>\n");
    }

    if(map.value(keys.at(4)).value.toBool()) // image
    {
        auto imgname = QString::number(m_Field->getInformation()->getAge()) +
                "." + config->ImageFileFormat().toLower();
        auto imgfile = dirname + QDir::separator() + imgname;
        auto imgbasefile =  QString(".") + QDir::separator() +
                QFileInfo(dirname).baseName() + QDir::separator() + imgname;
        auto pixmap = m_SceneView->getScene()->getSceneItem()->getPixmap();

        reportcontent.append(QString("<h3>%1</h3>\n").arg(imgcaption));

        if(QFile(imgfile).exists() || savePixmapToFile(pixmap, imgfile))
        {
            // проверка на exists - для совместимости картинок снапшотов и текущего поля
            reportcontent.append(QString("<img src = '%1' alt = '%2'>\n").
                                 arg(imgbasefile, imgname));
        }
    }

    if(map.value(keys.at(3)).value.toBool() || map.value(keys.at(4)).value.toBool())
        reportcontent.append("</div>\n");

    // rule, project files
    if(map.value(keys.at(6)).value.toBool() || map.value(keys.at(7)).value.toBool())
        reportcontent.append("<hr><div class = 'CONTENTDIV'>\n<ul>\n");

    if(map.value(keys.at(6)).value.toBool()) // rule
    {
        auto rulename = "rule." + RULE_FILE_EXTENSION.toLower();
        auto rulefile = dirname + QDir::separator() + rulename;
        auto rulebasefile =  QString(".") + QDir::separator() +
                QFileInfo(dirname).baseName() + QDir::separator() + rulename;

        if(saveRuleToFile(m_Field->getRule(), rulefile))
            reportcontent.append(QString("<li>%1:&nbsp;<a href = '%2'>%3</a></li>\n").
                                 arg(tr("Rule file"), rulebasefile, rulename));
    }

    if(map.value(keys.at(7)).value.toBool()) // project
    {
        auto savename = "project." + PROJECT_FILE_EXTENSION.toLower();
        auto savefile = dirname + QDir::separator() + savename;
        auto savebasefile =  QString(".") + QDir::separator() +
                QFileInfo(dirname).baseName() + QDir::separator() + savename;

        if(saveProjectToFile(savefile))
            reportcontent.append(QString("<li>%1:&nbsp;<a href = '%2'>%3</a></li>\n").
                                 arg(tr("Project file"), savebasefile, savename));
    }

    if(map.value(keys.at(6)).value.toBool() || map.value(keys.at(7)).value.toBool())
        reportcontent.append("</ul>\n</div>\n");

    reportcontent.append("<hr><div class = 'CONTENTDIV'>\n");
    reportcontent.append(QString("%1&nbsp;%2,&nbsp;format&nbsp;version:&nbsp;%3\n").
                         arg(APP_NAME, APP_VERSION, FORMAT_VERSION));
    reportcontent.append("</div><hr>");

    QString report = getTextFromRes(":/resources/html/report_body.html").
            arg(config->SceneBgColor(), caption, reportcontent);

    auto success_saving = textToFile(report, filename);

    if(!success_saving)
    {
        qCritical() << "Error at file saving:" << filename;
        QMessageBox::critical(this, tr("Error"),
                              tr("Error at file saving. Path: '%1'").arg(filename));
    }

    if(config->ReportAutoopen() && success_saving)
    {
        if(!OpenUrl(filename))
            QMessageBox::critical(this, tr("Error"), tr("Error at open URL:\n'%1'").arg(filename));
    }
    else if(success_saving)
        QMessageBox::information(this, tr("Report"), tr("Report completed. \n"
                                                        "Report file: '%1' \n"
                                                        "Additional report files: '%2'").
                                 arg(filename, dirname));

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotTasks()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    const QVector<QString> keys = {
        tr("00#_Tasks"),
        "01#_", tr("02#__Value:"),
        "03#_", tr("04#__Value:"),

        tr("05#_Activation"),
        tr("06#_Switch ON"),
    };
    QMap<QString, DialogValue> map = {
        {keys.at(0), {}},
        {keys.at(1), { QVariant::String,
                       tr("Pause at specified age (0 - never):"), "", "", DialogValueMode::Disabled}},
        {keys.at(2), {QVariant::Int, config->FieldPauseAtAge(), 0, 0}},
        {keys.at(3), { QVariant::String,
                       tr("Snapshot at every age (0 - never):"), "", "", DialogValueMode::Disabled}},
        {keys.at(4), {QVariant::Int, config->FieldSnapshotAtEveryTime(), 0, 0}},

        {keys.at(5), {}},
        {keys.at(6), {QVariant::Bool, config->UnsavedTasksEnabled()}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/tasks.svg", tr("Project tasks"), &map);
    dvl->resize(config->TasksWindowWidth(), config->TasksWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setTasksWindowWidth(size.width());
        config->setTasksWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    config->setFieldPauseAtAge(map.value(keys.at(2)).value.toInt());
    config->setFieldSnapshotAtEveryTime(map.value(keys.at(4)).value.toInt());

    config->setUnsavedTasksEnabled(map.value(keys.at(6)).value.toBool());
}

void MainWindow::slotRandomFill()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    const QVector<QString> keys = {
        tr("00#_Randomisation options"),
        tr("01#_Density of random fill, %")
    };
    QMap<QString, DialogValue> map = {
        {keys.at(0), {}},
        {keys.at(1), {QVariant::Int, config->FieldRandomisationValue(), 1, 100}}
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/cells.svg", tr("Randome filling"), &map);
    dvl->resize(config->RandomFillWindowWidth(), config->RandomFillWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setRandomFillWindowWidth(size.width());
        config->setRandomFillWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    config->setFieldRandomisationValue(map.value(keys.at(1)).value.toInt());

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto rg = QRandomGenerator::securelySeeded();
    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

    for(int x = xmin; x <= xmax; x++)
    {
        for(int y = ymin; y <= ymax; y++)
        {
            auto c = m_Field->getCell({x, y});
            auto ni = c->getNewInfo();
            auto oi = c->getOldInfo();

            c->clear();
            if(rg.bounded(0, 100) < config->FieldRandomisationValue())
            {
                ni->setState(Kernel::CellState::ALIVE);
                ni->setGeneration(1);
                oi->setState(Kernel::CellState::ALIVE);
                oi->setGeneration(1);
            }
        }
    }
    qDebug() << "Ramdom filled" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    m_Field->updateScene();

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotInvert()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

    for(int x = xmin; x <= xmax; x++)
        for(int y = ymin; y <= ymax; y++)
            m_Field->getCell({x, y})->invertState();

    qDebug() << "Inverted" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";

    m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotFlipHorizontal()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax_2 = ymin + (ymax - ymin) / 2;
    auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

    for(int x = xmin; x <= xmax; x++)
    {
        auto sy = ymax;
        for(int fy = ymin; fy <= ymax_2; fy++)
        {
            if(fy == sy) break;
            m_Field->getCell({x, fy})->flipInfo({x, sy});
            sy--;
        }
    }
    qDebug() << "Flipped horizontal" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";

    m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotFlipVertical()
{
    qDebug() << __func__;

    if(!validateSelectedCells()) return;
    auto scene = m_SceneView->getScene();
    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto time = QDateTime::currentMSecsSinceEpoch();
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto xmax_2 = xmin + (xmax - xmin) / 2;
    auto count = (xmax - xmin + 1) * (ymax - ymin + 1);

    for(int y = ymin; y <= ymax; y++)
    {
        auto sx = xmax;
        for(int fx = xmin; fx <= xmax_2; fx++)
        {
            if(fx == sx) break;
            m_Field->getCell({fx, y})->flipInfo({sx, y});
            sx--;
        }
    }
    qDebug() << "Flipped vertical" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";

    m_Field->updateScene();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotSelectAll()
{
    qDebug() << __func__;

    if(!validateScene()) return;
    auto scene = m_SceneView->getScene();

    auto firstcell = m_Field->getCell({0, 0});
    auto secondcell = m_Field->getCell({m_Field->width() - 1, m_Field->height() - 1});
    if(scene->getSelectedCell() != firstcell) scene->selectCell(firstcell);
    scene->MultiselectCell(secondcell);
}

void MainWindow::slotSelectedCellChanged(Cell *cell)
{
    setCellsActionsEnable(true);

    if(cell) m_LabelSelectedCell->setText(cell->objectName());
    else m_LabelSelectedCell->setText("-");
}

void MainWindow::slotSelectedCellsChanged(Cell *first, Cell *second)
{
    setCellsActionsEnable(true);

    if(!first || !second || first == second)
    {
        m_LabelSelectedCell->setText(!first ? "-" : first->objectName());
        return;
    }

    auto xmin = qMin(first->getIndex().x(), second->getIndex().x());
    auto xmax = qMax(first->getIndex().x(), second->getIndex().x());
    auto ymin = qMin(first->getIndex().y(), second->getIndex().y());
    auto ymax = qMax(first->getIndex().y(), second->getIndex().y());
    auto h = ymax - ymin + 1;
    auto w = xmax - xmin + 1;
    auto count = h * w;

    m_LabelSelectedCell->setText(QString("%1%2 : %3X%4 : %5").
                                 arg(first->objectName(),
                                     second->objectName(),
                                     QString::number(w),
                                     QString::number(h),
                                     QString::number(count)));
}

void MainWindow::slotLabelSelectedCellClick()
{
    auto scene = m_SceneView->getScene();
    if(!scene) return;

    auto cell = scene->getSelectedCell();
    slotShowCell(cell);
}

void MainWindow::slotCreateSnapshot()
{
    qDebug() << __func__;
    createSnapshot();

    QMessageBox::information(this, tr("Snapshot"),
                             tr("Snapshot of the field was created for %1 age.").
                             arg(QString::number(m_Field->getInformation()->getAge())));
}

void MainWindow::slotSelectSnapshot()
{
    qDebug() << __func__;
    if(!m_Snapshots->count())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Snapshot list is empty."));
        return;
    }

    QVector<QString> keys =
    { tr("00#_Available snapshots: %1").arg(QString::number(m_Snapshots->count())),
      tr("01#_Age: "),
      tr("02#_Delete snapshots after the selected") };

    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList, m_Snapshots->keys().at(0), 0,
                    m_Snapshots->keys(), DialogValueMode::OneFromList}},
      {keys.at(2), {QVariant::Bool, false}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/next_snapshot.svg",
                                    tr("Select snapshot"), &map);
    dvl->resize(config->SnapshotWindowWidth(), config->SnapshotWindowHeight());
    QObject::connect(dvl, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setSnapshotWindowWidth(size.width());
        config->setSnapshotWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    auto key = map.value(keys.at(1)).value.toString();
    auto jdoc = m_Snapshots->getSnapshot(key).document;
    loadSnapshot(&jdoc);

    if(map.value(keys.at(2)).value.toBool()) m_Snapshots->clearAfter(key);
}

void MainWindow::slotLoadFirstSnapshot()
{
    qDebug() << __func__;

    if(!m_Snapshots->count()) return;

    auto jdoc = m_Snapshots->getFirst().document;
    loadSnapshot(&jdoc);
}

void MainWindow::slotLoadProject()
{
    qDebug() << __func__;
    stopFieldCalculating();

    auto fileext = PROJECT_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getOpenFileName(this, tr("Load project"), config->PathPojectsDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    QByteArray data;
    bool ok = readUncompressData(&data, filename);
    // если не смогли распаковать
    if(!ok)
    {
        qWarning() << "Data uncompressing from file failed";
        qDebug() << "File is not compressed? Will try to read it";
        data = fileToText(filename, &ok).toUtf8();
    }
    if(!ok)
    {
        qCritical() << "Error at loading data from file";
        QMessageBox::critical(this, tr("Error"), tr("Error at loading data from file: '%1'").arg(filename));
        return;
    }

    QJsonObject root_object;
    if(!getJsonRootObject(data, &root_object))
    {
        QMessageBox::critical(this, tr("Error"), tr("Data error. Data is not Json document. \n File: '%1'").arg(filename));
        return;
    }
    if(!checkJsonDocumentVersion(&root_object)) return;

    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    if(!loadProjectFromJsonObject(&root_object))
    {
        QMessageBox::critical(this, tr("Error"), tr("Error at project loading."));
        return;
    }

    setWindowTitle(QString("%1 %2 <%3> [%4 X %5 X %6]").
                   arg(APP_NAME, APP_VERSION, m_Field->getRule()->objectName(),
                       QString::number(m_Field->width()),
                       QString::number(m_Field->height()),
                       QString::number(config->SceneCellSize())));

    m_SceneView->zoomer()->resetZoom();
    m_LabelFieldAvCalc->setText("0 ms");
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

    m_Snapshots->clear();
    createScene();
    m_Field->updateScene();

    setMainActionsEnable(true);
    if(config->WindowShowFieldInfo()) slotInfoField();
}

void MainWindow::slotSaveProject()
{
    qDebug() << __func__;
    auto fileext = PROJECT_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save project"), config->PathPojectsDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto dot_fileext = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileext, Qt::CaseInsensitive)) filename.append(dot_fileext);

    saveProjectToFile(filename);

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

bool MainWindow::saveProjectToFile(const QString& path)
{
    qDebug() << __func__;
    auto datetime = QDateTime::currentDateTime().toString(config->DateTimeFormat());
    QJsonDocument document;
    QJsonObject obj_root {{"DateTime", datetime},
                          {"Application", APP_NAME},
                          {"Version", FORMAT_VERSION}};

    FieldToJsonObject(&obj_root);
    RuleToJsonObject(m_Field->getRule(),&obj_root);

    auto firstcell = m_Field->getCell({0, 0});
    auto secondcell = m_Field->getCell({m_Field->width() - 1, m_Field->height() - 1});
    CellsToJsonObject(&obj_root, firstcell, secondcell, false);

    document.setObject(obj_root);
    auto json_mode = config->JsonCompactMode() ? QJsonDocument::Compact : QJsonDocument::Indented;
    auto text = document.toJson(json_mode);

    bool ok;
    if(config->ProjectFileCompression()) ok = writeCompressData(text, path);
    else ok = textToFile(text, path);
    if(!ok)
    {
        qCritical() << "Data writing error:" << path;
        QMessageBox::critical(this, tr("Error"), tr("Data writing error. \n File: '%1'").arg(path));
        return false;
    }
    return true;
}

void MainWindow::slotNewRule()
{
    qDebug() << __func__;
    auto rule = new FieldRule();

    auto der = new DialogEditRules(this, rule);
    der->resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());
    QObject::connect(der, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setEditRulesWindowWidth(size.width());
        config->setEditRulesWindowHeight(size.height());
    });

    if(der->exec()) saveRule(rule);
    rule->deleteLater();
}

void MainWindow::slotLoadEditRule()
{
    qDebug() << __func__;
    auto fileext = RULE_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getOpenFileName(this, tr("Load rule"), config->PathRulesDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto rule = new FieldRule;
    if(!RuleFromFilePath(rule, filename))
    {
        rule->deleteLater();
        return;
    }

    auto der = new DialogEditRules(this, rule);
    der->resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());
    QObject::connect(der, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setEditRulesWindowWidth(size.width());
        config->setEditRulesWindowHeight(size.height());
    });

    if(der->exec()) saveRule(rule);
    rule->deleteLater();
}

void MainWindow::slotInfoRule()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    auto title = tr("Information about current rule");
    if(findPreviousWindowCopy(title)) return;

    auto dic = new DialogInfoContent(this, title);
    dic->resize(config->InfoRuleWindowWidth(), config->InfoRuleWindowHeight());
    QObject::connect(dic, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setInfoRuleWindowWidth(size.width());
        config->setInfoRuleWindowHeight(size.height());
    });

    QString content = tr("## Properties\n");
    for(auto s: m_Field->getRule()->PropertiesToString().split('\n'))
        content.append(QString(" - <pre><b>%1</b></pre>\n").arg(s));

    content.append("\n --- \n");

    content.append(tr("## Activities\n"));
    for(auto a: m_Field->getRule()->getActivity())
        content.append(QString(" - <pre><b>%1</b></pre>\n").
                       arg(ActivityElementToString(a)));

    dic->setMarkdownContent(content);
    dic->show();
}

void MainWindow::slotImportRule()
{
    qDebug() << __func__;

    if(!validateScene()) return;

    auto rule = new FieldRule(m_Field->getRule());

    auto der = new DialogEditRules(this, rule);
    der->resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());
    QObject::connect(der, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setEditRulesWindowWidth(size.width());
        config->setEditRulesWindowHeight(size.height());
    });

    if(der->exec()) saveRule(rule);
    rule->deleteLater();
}

void MainWindow::slotAbout()
{
    auto content = getTextFromRes(":/resources/html/about_body.html").
            arg(APP_NAME, APP_VERSION, FORMAT_VERSION, GIT_VERS, BUILD_DATE, getSystemInfo(), QT_VERSION_STR);

    auto title = tr("About %1").arg(APP_NAME);
    if(findPreviousWindowCopy(title)) return;

    auto dic = new DialogInfoContent(this, title);
    dic->resize(config->InfoWindowWidth(), config->InfoWindowHeight());
    QObject::connect(dic, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setInfoWindowWidth(size.width());
        config->setInfoWindowHeight(size.height());
    });
    dic->setHtmlContent(content);
    dic->show();
}

void MainWindow::slotHelp()
{
    auto title = tr("Help");
    if(findPreviousWindowCopy(title)) return;

    auto dic = new DialogInfoContent(this, title);
    dic->resize(config->HelpWindowWidth(), config->HelpWindowHeight());
    QObject::connect(dic, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setHelpWindowWidth(size.width());
        config->setHelpWindowHeight(size.height());
    });
    dic->setMarkdownSource(config->HelpPage());
    dic->show();
}

void MainWindow::slotCellMonitor()
{
    auto title = tr("Cells monitor");
    if(findPreviousWindowCopy(title)) return;

    auto dcm = new DialogCellMonitor(this, title, m_SceneView->getScene());
    dcm->resize(config->CellMonitorWindowWidth(), config->CellMonitorWindowHeight());
    QObject::connect(dcm, &DialogBody::signalSizeChanged, [=](QSize size)
    {
        config->setCellMonitorWindowWidth(size.width());
        config->setCellMonitorWindowHeight(size.height());
    });
    dcm->show();
}

void MainWindow::slotFieldAvCalc(qreal value)
{
    if(m_LabelFieldAvCalc->styleSheet() != MW_LABEL_STYLE)
        m_LabelFieldAvCalc->setStyleSheet(MW_LABEL_STYLE);

    m_LabelFieldAvCalc->setText(tr("%1 ms").arg(QString::number(value, 'f', 1)));
}

void MainWindow::slotFieldAvCalcUp(qreal value)
{
    if(m_LabelFieldAvCalc->styleSheet() != MW_LABEL_STYLE_UP)
        m_LabelFieldAvCalc->setStyleSheet(MW_LABEL_STYLE_UP);

    m_LabelFieldAvCalc->setText(tr("%1 ms").arg(QString::number(value, 'f', 1)));
}

void MainWindow::slotFieldAvCalcDown(qreal value)
{
    if(m_LabelFieldAvCalc->styleSheet() != MW_LABEL_STYLE_DOWN)
        m_LabelFieldAvCalc->setStyleSheet(MW_LABEL_STYLE_DOWN);

    m_LabelFieldAvCalc->setText(tr("%1 ms").arg(QString::number(value, 'f', 1)));
}

void MainWindow::slotAverageDrawUp(qreal value)
{
    if(m_LabelSceneAvDraw->styleSheet() != MW_LABEL_STYLE_UP)
        m_LabelSceneAvDraw->setStyleSheet(MW_LABEL_STYLE_UP);

    m_LabelSceneAvDraw->setText(tr("%1 ms").arg(QString::number(value, 'f', 1)));
}

void MainWindow::slotAverageDrawDown(qreal value)
{
    if(m_LabelSceneAvDraw->styleSheet() != MW_LABEL_STYLE_DOWN)
        m_LabelSceneAvDraw->setStyleSheet(MW_LABEL_STYLE_DOWN);

    m_LabelSceneAvDraw->setText(tr("%1 ms").arg(QString::number(value, 'f', 1)));
}

void MainWindow::slotFieldAge(uint value) { m_LabelFieldAge->setText(QString::number(value)); }
QProgressBar *MainWindow::ProgressBar() const { return m_ProgressBar; }
