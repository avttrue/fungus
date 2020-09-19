#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"
#include "jdocumentlist.h"
#include "scene/scene.h"
#include "scene/sceneitem.h"
#include "scene/sceneview.h"
#include "graphicsviewzoomer.h"
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
    m_Snapshots = new JDocumentList(this);
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
    m_ActionZoomInScene->setShortcut(Qt::CTRL + Qt::Key_Plus);
    addShortcutToToolTip(m_ActionZoomInScene);
    m_ActionZoomInScene->setAutoRepeat(false);
    m_ActionZoomInScene->setEnabled(false);

    m_ActionZoomOutScene = new QAction(QIcon(":/resources/img/zoom_out.svg"), tr("Zoom OUT"), this);
    QObject::connect(m_ActionZoomOutScene, &QAction::triggered, this, &MainWindow::slotSceneZoomOut);
    m_ActionZoomOutScene->setShortcut(Qt::CTRL + Qt::Key_Minus);
    addShortcutToToolTip(m_ActionZoomOutScene);
    m_ActionZoomOutScene->setAutoRepeat(false);
    m_ActionZoomOutScene->setEnabled(false);

    m_ActionZoomUndoScene = new QAction(QIcon(":/resources/img/zoom_undo.svg"), tr("Zoom UNDO"), this);
    QObject::connect(m_ActionZoomUndoScene, &QAction::triggered, this, &MainWindow::slotZoomUndoScene);
    m_ActionZoomUndoScene->setAutoRepeat(false);
    m_ActionZoomUndoScene->setEnabled(false);

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

    m_ActionSaveCellsToClipbord = new QAction(QIcon(":/resources/img/copy.svg"), tr("Copy selected cells to clipboard"), this);
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

    m_ActionCellMonitor = new QAction(QIcon(":/resources/img/cell_monitor.svg"), tr("Cells minitor"), this);
    QObject::connect(m_ActionCellMonitor, &QAction::triggered, this, &MainWindow::slotCellMonitor);
    m_ActionCellMonitor->setAutoRepeat(false);
    m_ActionCellMonitor->setEnabled(false);

    m_ActionClearCells = new QAction(QIcon(":/resources/img/delete.svg"), tr("Full cell clearing"), this);
    QObject::connect(m_ActionClearCells, &QAction::triggered, this, &MainWindow::slotClearCells);
    m_ActionClearCells->setShortcut(Qt::CTRL + Qt::Key_D);
    addShortcutToToolTip(m_ActionClearCells);
    m_ActionClearCells->setAutoRepeat(false);
    m_ActionClearCells->setEnabled(false);

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
    m_TbMain->addAction(m_ActionNewProject);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionLoadProject);
    m_TbMain->addAction(m_ActionSaveProject);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionSaveImageToFile);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionZoomUndoScene);
    m_TbMain->addAction(m_ActionZoomInScene);
    m_TbMain->addAction(m_ActionZoomOutScene);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionEditCell);
    m_TbMain->addAction(m_ActionInfoCell);
    m_TbMain->addAction(m_ActionCellMonitor);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionInfoField);
    m_TbMain->addSeparator();
    m_TbMain->addAction(m_ActionStepStop);
    m_TbMain->addAction(m_ActionRun);
    m_TbMain->addWidget(new WidgetSpacer(this));
    m_TbMain->addAction(actionSetup);
    m_TbMain->addSeparator();
    m_TbMain->addAction(actionHelp);
    m_TbMain->addAction(actionAbout);
    m_TbMain->addAction(actionQt);
    m_TbMain->addAction(actionExit);
    addToolBar(Qt::TopToolBarArea, m_TbMain);

    // тулбар дополнительный
    m_TbActions = new QToolBar(this);
    m_TbActions->setMovable(false);
    m_TbActions->setOrientation(Qt::Vertical);
    m_TbActions->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    m_TbActions->addWidget(m_BtnMenuRules);
    m_TbActions->addSeparator();
    m_TbActions->addAction(m_ActionSaveCellsToClipbord);
    m_TbActions->addAction(m_ActionLoadCellsFromClipbord);
    m_TbActions->addSeparator();
    m_TbActions->addAction(m_ActionSaveCellsToPreset);
    m_TbActions->addAction(m_ActionLoadCellsFromPreset);
    m_TbActions->addSeparator();
    m_TbActions->addAction(m_ActionCreateSnapshot);
    m_TbActions->addAction(m_ActionSelectSnapshot);
    m_TbActions->addSeparator();
    m_TbActions->addAction(m_ActionSelectAll);
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
    if(!m_SceneView->getScene())
    {
        qCritical() << "Scene not created";
        return;
    }

    if(!m_Field->isCalculating())
    {
        if(config->SceneFirstSnapshot() && !m_Snapshots->count())
            createSnapshot();
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
    if(!m_SceneView->getScene())
    {
        qCritical() << "Scene not created";
        return;
    }

    if(!m_Field->isCalculating())
    {
        if(config->SceneFirstSnapshot() && !m_Field->getInformation()->getAge())
            createSnapshot();
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
    m_ActionZoomInScene->setEnabled(enable);
    m_ActionZoomOutScene->setEnabled(enable);
    m_ActionZoomUndoScene->setEnabled(enable);
    m_ActionStepStop->setEnabled(enable);
    m_ActionInfoField->setEnabled(enable);
    m_ActionCellMonitor->setEnabled(enable);
    m_ActionSaveImageToFile->setEnabled(enable);
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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        qCritical() << "Scene not created";
        return false;
    }

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
        if (!answer) return false;

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
        /* {Type, SelfState, Target, TargetState, Operand, Operator, OperandValue} */
        QJsonObject obj_act;
        obj_act.insert("Type", a.at(0).toInt());
        obj_act.insert("SelfState", a.at(1).toInt());
        obj_act.insert("Target", a.at(2).toInt());
        obj_act.insert("TargetState", a.at(3).toInt());
        obj_act.insert("Operand", a.at(4).toInt());
        obj_act.insert("Operator", a.at(5).toInt());
        obj_act.insert("OperandValue", a.at(6).toInt());
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

    if(!obj_rule.contains("CurseTime"))
    { qCritical() << "JsonValue 'CurseTime' is absent"; return false; }
    rule->setCurseTime(obj_rule["CurseTime"].toInt());

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

void MainWindow::saveRuleToFile(FieldRule *rule)
{
    qDebug() << __func__;
    auto fileext = RULE_FILE_EXTENSION.toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save rule"), config->PathRulesDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    auto dot_fileext = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileext, Qt::CaseInsensitive)) filename.append(dot_fileext);

    auto datetime = QDateTime::currentDateTime().toString(config->DateTimeFormat());
    QJsonDocument document;
    QJsonObject obj_root {{"DateTime", datetime},
                          {"Application", APP_NAME},
                          {"Version", FORMAT_VERSION}};
    RuleToJsonObject(rule, &obj_root);
    document.setObject(obj_root);

    auto json_mode = config->JsonCompactMode() ? QJsonDocument::Compact : QJsonDocument::Indented;
    auto text = document.toJson(json_mode);
    if(!textToFile(text, filename))
        QMessageBox::critical(this, tr("Error"), tr("Data writing error. \n File: '%1'").arg(filename));
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
    if(!m_SceneView->getScene())
    {
        qCritical() << "Scene not created";
        return;
    }

    if(m_Field->getInformation()->getAge() == 0 &&
            config->SceneFirstSnapshotClearList())
        m_Snapshots->clearList();

    auto name = QString("Age: %1").arg(QString::number(m_Field->getInformation()->getAge()));
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
    m_Snapshots->addDocument(name, document);
}

void MainWindow::loadSnapshot(QJsonDocument* document)
{
    qDebug() << __func__;
    if(!m_SceneView->getScene())
    {
        qCritical() << "Scene not created";
        return;
    }

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
    if(!file.open(QIODevice::ReadOnly)) return false;

    auto time = QDateTime::currentMSecsSinceEpoch();
    QByteArray ba = file.readAll();
    file.close();
    if(!ba.size()) return false;

    *data = qUncompress(ba);
    qDebug() << "Read & uncompress data:" << ba.size() << "bytes in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    return data->size();
}

void MainWindow::slotSetup()
{
    stopFieldCalculating();
    const QVector<QString> keys = {tr("00#_Common options"),
                                   tr("01#_Date and time format"),
                                   tr("02#_Write logs to file (restart required)"),
                                   tr("03#_Buttons size"),
                                   tr("04#_Rewrite resources at start"),
                                   tr("05#_Compress data in project files"),
                                   tr("06#_Scene options"),
                                   tr("07#_Antialiasing"),
                                   tr("08#_Update mode"),
                                   tr("09#_Background color"),
                                   tr("10#_Selection color"),
                                   tr("11#_Zoom factor"),
                                   tr("12#_Cells options"),
                                   tr("13#_Alive cell color"),
                                   tr("14#_Dead cell color"),
                                   tr("15#_Cursed cell color"),
                                   tr("16#_Indicate value alive cell age"),
                                   tr("17#__Brightness"),
                                   tr("18#__Maximal factor"),
                                   tr("19#__Age diapason"),
                                   tr("20#_Indicate value cursed cell age"),
                                   tr("21#__Brightness"),
                                   tr("22#__Maximal factor"),
                                   tr("23#__Age diapason"),
                                   tr("24#_Field options"),
                                   tr("25#_Create first snapshot"),
                                   tr("26#_Clear snapshot list at first snapshot"),
                                   tr("27#_Minimum pause at calculating (ms)"),
                                   tr("28#_Copy to clipboard except dead cells"),
                                   tr("29#_Save to preset except dead cells"),
                                   tr("30#_Field thread priority"),
                                  };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
     {keys.at(2), {QVariant::Bool, config->WriteLogsToFile()}},
     {keys.at(3), {QVariant::Int, config->ButtonSize(), 16, 100}},
     {keys.at(4), {QVariant::Bool, config->RewriteResource()}},
     {keys.at(5), {QVariant::Bool, config->ProjectFileCompression()}},
     {keys.at(6), {}},
     {keys.at(7), {QVariant::Bool, config->SceneViewAntialiasing()}},
     {keys.at(8), {QVariant::StringList, config->SceneViewUpdateMode(), 0, SCENE_VIEW_UPDATE_MODES, DialogValueMode::OneFromList}},
     {keys.at(9), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(10), {QVariant::String, config->SceneSelectColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(11), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
     {keys.at(12), {}},
     {keys.at(13), {QVariant::String, config->SceneCellAliveColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(14), {QVariant::String, config->SceneCellDeadColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(15), {QVariant::String, config->SceneCellCurseColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(16), {QVariant::Bool, config->CellAliveAgeIndicate()}},
     {keys.at(17), {QVariant::StringList, config->CellAliveAgeIndicBright(), 0, SCENE_CELL_BRIGHTNESS_VALUES, DialogValueMode::OneFromList}},
     {keys.at(18), {QVariant::Int, config->CellAliveAgeIndicFactor(), 150, 1000}},
     {keys.at(19), {QVariant::Int, config->CellAliveAgeIndicDiapason(), 2, 50}},
     {keys.at(20), {QVariant::Bool, config->CellCurseAgeIndicate()}},
     {keys.at(21), {QVariant::StringList, config->CellCurseAgeIndicBright(), 0, SCENE_CELL_BRIGHTNESS_VALUES, DialogValueMode::OneFromList}},
     {keys.at(22), {QVariant::Int, config->CellCurseAgeIndicFactor(), 150, 1000}},
     {keys.at(23), {QVariant::Int, config->CellCurseAgeIndicDiapason(), 2, 50}},
     {keys.at(24), {}},
     {keys.at(25), {QVariant::Bool, config->SceneFirstSnapshot()}},
     {keys.at(26), {QVariant::Bool, config->SceneFirstSnapshotClearList()}},
     {keys.at(27), {QVariant::Int, config->SceneCalculatingMinPause(), 0, 10000}},
     {keys.at(28), {QVariant::Bool, config->CopyToClipboardExceptDead()}},
     {keys.at(29), {QVariant::Bool, config->SaveToPresetExceptDead()}},
     {keys.at(30), {QVariant::StringList, config->SceneFieldThreadPriority(), 0, SCENE_FIELD_THREAD_PRIORITIES, DialogValueMode::OneFromList}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/setup.svg", tr("Settings"), &map);
    if(!dvl->exec()) return;

    // common
    config->setDateTimeFormat(map.value(keys.at(1)).value.toString());
    config->setWriteLogsToFile(map.value(keys.at(2)).value.toBool());
    config->setButtonSize(map.value(keys.at(3)).value.toInt());
    config->setRewriteResource(map.value(keys.at(4)).value.toInt());
    config->setProjectFileCompression(map.value(keys.at(5)).value.toBool());
    // scene
    config->setSceneViewAntialiasing(map.value(keys.at(7)).value.toBool());
    config->setSceneViewUpdateMode(map.value(keys.at(8)).value.toString());
    config->setSceneBgColor(map.value(keys.at(9)).value.toString());
    config->setSceneSelectColor(map.value(keys.at(10)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(11)).value.toDouble());
    // cell
    config->setSceneCellAliveColor(map.value(keys.at(13)).value.toString());
    config->setSceneCellDeadColor(map.value(keys.at(14)).value.toString());
    config->setSceneCellCurseColor(map.value(keys.at(15)).value.toString());
    config->setCellAliveAgeIndicate(map.value(keys.at(16)).value.toBool());
    config->setCellAliveAgeIndicBright(map.value(keys.at(17)).value.toString());
    config->setCellAliveAgeIndicFactor(map.value(keys.at(18)).value.toInt());
    config->setCellAliveAgeIndicDiapason(map.value(keys.at(19)).value.toInt());
    config->setCellCurseAgeIndicate(map.value(keys.at(20)).value.toBool());
    config->setCellCurseAgeIndicBright(map.value(keys.at(21)).value.toString());
    config->setCellCurseAgeIndicFactor(map.value(keys.at(22)).value.toInt());
    config->setCellCurseAgeIndicDiapason(map.value(keys.at(23)).value.toInt());
    // field
    config->setSceneFirstSnapshot(map.value(keys.at(25)).value.toBool());
    config->setSceneFirstSnapshotClearList(map.value(keys.at(26)).value.toBool());
    config->setSceneCalculatingMinPause(map.value(keys.at(27)).value.toInt());
    config->setCopyToClipboardExceptDead(map.value(keys.at(28)).value.toBool());
    config->setSaveToPresetExceptDead(map.value(keys.at(29)).value.toBool());
    config->setSceneFieldThreadPriority(map.value(keys.at(30)).value.toString());

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
    }
}

void MainWindow::slotEditCell()
{
    qDebug() << __func__;
    stopFieldCalculating();

    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionEditCell->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    if(!firstcell)
    {
        m_ActionEditCell->setDisabled(true);
        qDebug() << "Cell for editing not selected";
        return;
    }

    auto secondcell = scene->getSecondSelectedCell();
    auto multyselection =  !secondcell ? false : true;
    auto cni = firstcell->getNewInfo();
    auto statelist = listKernelEnum("CellState");

    QVector<QString> keys =
    { tr("00#_Cell properties"),
      tr("01#_State"),
      tr("02#_Age"),
      tr("03#_Generation")};
    if(multyselection) keys.append(
                { tr("04#_Group operations"),
                  tr("05#_Apply to all"),
                  tr("06#_Exclude 'State' property")});

    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList,
                    getNameKernelEnum("CellState", static_cast<int>(cni->getState())), 0,
                    statelist, DialogValueMode::OneFromList}},
      {keys.at(2), {QVariant::Int, cni->getAge(), 0, 0}},
      {keys.at(3), {QVariant::Int, cni->getGeneration(), 0, 0}}
    };
    if(multyselection)
    {
        QMap<QString, DialogValue> addmap =
        { {keys.at(4), {}},
          {keys.at(5), {QVariant::Bool, true}},
          {keys.at(6), {QVariant::Bool, false}}};
        map.insert(addmap);
    }

    auto dvl = new DialogValuesList(this, ":/resources/img/point.svg",
                                    tr("Edit cell %1").arg(firstcell->objectName()), &map);
    if(!dvl->exec()) return;

    if(multyselection && map.value(keys.at(5)).value.toBool())
    {
        auto exclude_state = map.value(keys.at(6)).value.toBool();
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
                if(!exclude_state)
                    cni->setState(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(1)).value.toString())));
                cni->setAge(map.value(keys.at(2)).value.toUInt());
                cni->setGeneration(map.value(keys.at(3)).value.toUInt());
                c->applyInfo();
            }
        }
        qDebug() << "Group of" << count << "cells editing complete in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
    }
    else
    {
        cni->setState(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(1)).value.toString())));
        cni->setAge(map.value(keys.at(2)).value.toUInt());
        cni->setGeneration(map.value(keys.at(3)).value.toUInt());
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
        tr("01#_Size"),
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

    m_Snapshots->clearList();
    createScene();
    if(random) m_Field->updateScene();

    setWindowTitle(QString("%1 %2 <%3> [%4 X %5 X %6]").
                   arg(APP_NAME, APP_VERSION, currentrule,
                       QString::number(m_Field->width()),
                       QString::number(m_Field->height()),
                       QString::number(config->SceneCellSize())));

    setMainActionsEnable(true);
    setCellsActionsEnable(true);

    m_SceneView->zoomer()->Zoom(-1.0);
    m_LabelFieldAvCalc->setText("0 ms");
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

    if(config->WindowShowFieldInfo()) slotInfoField();
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
{
    m_SceneView->zoomer()->Zoom(ZOOM_FACTOR_RESET);
}

void MainWindow::slotSaveCellsToClipbord()
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionSaveCellsToClipbord->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();

    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionSaveCellsToClipbord->setDisabled(true);
        qDebug() << "Cells for saving not selected";
        return;
    }

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto text = CellsToJsonText(firstcell, secondcell, config->CopyToClipboardExceptDead());
    auto clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotLoadCellsFromClipbord()
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionLoadCellsFromClipbord->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionSaveCellsToPreset->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionSaveCellsToPreset->setDisabled(true);
        qDebug() << "Cells for saving not selected";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionLoadCellsFromPreset->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionClearCells->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionClearCells->setDisabled(true);
        qDebug() << "Target cell not selected";
        return;
    }

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
    {
        for(int y = ymin; y <= ymax; y++)
        {
            auto c = m_Field->getCell({x, y});
            c->clear();
        }
    }
    qDebug() << "Cleared" << count << "cells in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";

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

    QObject::connect(dci, &DialogCellInformation::signalShowCell, this, &MainWindow::slotShowCell);
    dci->show();
}

void MainWindow::slotInfoField()
{
    if(!m_Field) { m_ActionInfoField->setDisabled(true); return; }

    auto title = tr("Field");
    if(findPreviousWindowCopy(title)) return;

    auto dfi = new DialogFieldInformation(this, title, m_Field);
    dfi->show();
}

void MainWindow::slotShowCell(Cell *cell)
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        qCritical() << "Scene not created";
        return;
    }

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
    if(!m_SceneView->getScene())
    {
        qCritical() << "Scene not created";
        return;
    }

    auto fileext = config->ImageFileFormat().toLower();
    auto filename = QFileDialog::getSaveFileName(this, tr("Save image"), config->LastDir(),
                                                 tr("%1 files (*.%2)").arg(fileext.toUpper(), fileext));

    if(filename.isNull() || filename.isEmpty()) return;

    config->setLastDir(QFileInfo(filename).dir().path());

    auto dot_fileformat = QString(".%1").arg(fileext);
    if(!filename.endsWith(dot_fileformat, Qt::CaseInsensitive)) filename.append(dot_fileformat);

    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);

    auto pixmap = m_SceneView->getScene()->getSceneItem()->getPixmap();
    if(!pixmap->save(filename, fileext.toUpper().toLatin1().constData()))
        QMessageBox::critical(this, tr("Error"),
                              tr("Error at file saving. Path: '%1'").arg(filename));

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotRandomFill()
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionRandomFill->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionRandomFill->setDisabled(true);
        qDebug() << "Target cells not selected";
        return;
    }

    const QVector<QString> keys = {
        tr("00#_Randomisation options"),
        tr("01#_Density of random fill, %")
    };
    QMap<QString, DialogValue> map = {
        {keys.at(0), {}},
        {keys.at(1), {QVariant::Int, config->FieldRandomisationValue(), 1, 100}}
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/cells.svg", tr("Randome filling"), &map);
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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionInvert->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionInvert->setDisabled(true);
        qDebug() << "Target cells not selected";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionFlipHorizontal->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionInvert->setDisabled(true);
        qDebug() << "Target cells not selected";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionFlipVertical->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)
    {
        m_ActionInvert->setDisabled(true);
        qDebug() << "Target cells not selected";
        return;
    }

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
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionSelectAll->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

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
    stopFieldCalculating();
    setMainActionsEnable(false);
    setCellsActionsEnable(false);
    createSnapshot();
    setMainActionsEnable(true);
    setCellsActionsEnable(true);

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
      "01#_" };

    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList, m_Snapshots->keys().at(0), 0,
                    m_Snapshots->keys(),
                    DialogValueMode::OneFromList}} };

    auto dvl = new DialogValuesList(this, ":/resources/img/next_snapshot.svg",
                                    tr("Select snapshot"), &map);
    if(!dvl->exec()) return;

    auto key = map.value(keys.at(1)).value.toString();
    auto jdoc = m_Snapshots->getDocument(key);
    loadSnapshot(&jdoc);
}

void MainWindow::slotLoadFirstSnapshot()
{
    qDebug() << __func__;

    if(!m_Snapshots->count()) return;

    auto jdoc = m_Snapshots->getFirstDocument();
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
    bool ok;
    if(config->ProjectFileCompression()) ok = readUncompressData(&data, filename);
    else data = fileToText(filename, &ok).toUtf8();
    if(!ok)
    {
        qCritical() << "Error at loading data from file";
        QMessageBox::critical(this, tr("Error"),
                              tr("Error at loading data from file: '%1'").arg(filename));
        return;
    }

    QJsonObject root_object;
    if(!getJsonRootObject(data, &root_object))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Data error. Data is not Json document. \n File: '%1'").arg(filename));
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

    m_SceneView->zoomer()->Zoom(-1.0);
    m_LabelFieldAvCalc->setText("0 ms");
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

    m_Snapshots->clearList();
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
    if(config->ProjectFileCompression()) ok = writeCompressData(text, filename);
    else ok = textToFile(text, filename);
    if(!ok)
    {
        qCritical() << "Data writing error";
        QMessageBox::critical(this, tr("Error"), tr("Data writing error. \n File: '%1'").arg(filename));
    }

    setMainActionsEnable(true);
    setCellsActionsEnable(true);
}

void MainWindow::slotNewRule()
{
    qDebug() << __func__;
    auto rule = new FieldRule();
    auto der = new DialogEditRules(this, rule);
    if(der->exec()) saveRuleToFile(rule);
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
    if(der->exec()) saveRuleToFile(rule);
    rule->deleteLater();
}

void MainWindow::slotInfoRule()
{
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionInfoRule->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto title = tr("Information about current rule");
    if(findPreviousWindowCopy(title)) return;

    auto dhc = new DialogInfoContent(this, title);

    QString content = "## Properties\n";
    for(auto s: m_Field->getRule()->PropertiesToString().split('\n'))
        content.append(QString(" - <pre><b>%1</b></pre>\n").arg(s));

    content.append("\n --- \n");

    content.append("## Activities\n");
    for(auto a: m_Field->getRule()->getActivity())
        content.append(QString(" - <pre><b>%1</b></pre>\n").
                       arg(ActivityElementToString(a)));

    dhc->setMarkdownContent(content);
    dhc->show();
}

void MainWindow::slotImportRule()
{
    qDebug() << __func__;
    auto scene = m_SceneView->getScene();
    if(!scene)
    {
        m_ActionImportRule->setDisabled(true);
        qCritical() << "Scene not created";
        return;
    }

    auto rule = new FieldRule(m_Field->getRule());
    auto der = new DialogEditRules(this, rule);
    if(der->exec()) saveRuleToFile(rule);
    rule->deleteLater();
}

void MainWindow::slotAbout()
{
    auto content = getTextFromRes(":/resources/html/about_body.html").
            arg(APP_NAME, APP_VERSION, GIT_VERS, BUILD_DATE, getSystemInfo(), QT_VERSION_STR);

    auto title = tr("About %1").arg(APP_NAME);
    if(findPreviousWindowCopy(title)) return;

    auto dhc = new DialogInfoContent(this, title);
    dhc->setHtmlContent(content);
    dhc->show();
}

void MainWindow::slotHelp()
{
    auto title = tr("Help");
    if(findPreviousWindowCopy(title)) return;

    auto dhc = new DialogInfoContent(this, title);
    dhc->setMarkdownSource("qrc:/resources/md/help/doc_en.md");
    dhc->show();
}

void MainWindow::slotCellMonitor()
{
    auto title = tr("Cells monitor");
    if(findPreviousWindowCopy(title)) return;

    auto dcm = new DialogCellMonitor(this, title, m_SceneView->getScene());
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
