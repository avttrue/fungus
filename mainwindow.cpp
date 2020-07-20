#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"
#include "scene/scene.h"
#include "scene/sceneitem.h"
#include "scene/sceneview.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogvalueslist.h"
#include "dialogs/dialogcellinformation.h"
#include "dialogs/dialogfieldinformation.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellinformation.h"
#include "field/fieldrule.h"
#include "field/fieldinformation.h"

#include <QDebug>
#include <QAction>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ThreadField(nullptr),
      m_Field(nullptr)
{    
    setWindowIcon(QIcon(":/resources/img/flora.svg"));
    setWindowTitle(QString("%1 %2").arg(APP_NAME, APP_VERS));
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

    auto actionExit = new QAction(QIcon(":/resources/img/exit.svg"), tr("Exit"), this);
    QObject::connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    auto actionNew = new QAction(QIcon(":/resources/img/asterisk.svg"), tr("New project"), this);
    QObject::connect(actionNew, &QAction::triggered, this, &MainWindow::slotNewProject);
    actionNew->setShortcut(Qt::CTRL + Qt::Key_N);

    m_ActionZoomInScene = new QAction(QIcon(":/resources/img/zoom_in.svg"), tr("Zoom IN"), this);
    QObject::connect(m_ActionZoomInScene, &QAction::triggered, this, &MainWindow::slotSceneZoomIn);
    m_ActionZoomInScene->setEnabled(false);

    m_ActionZoomOutScene = new QAction(QIcon(":/resources/img/zoom_out.svg"), tr("Zoom OUT"), this);
    QObject::connect(m_ActionZoomOutScene, &QAction::triggered, this, &MainWindow::slotSceneZoomOut);
    m_ActionZoomOutScene->setEnabled(false);

    m_ActionZoomUndoScene = new QAction(QIcon(":/resources/img/zoom_undo.svg"), tr("Zoom UNDO"), this);
    QObject::connect(m_ActionZoomUndoScene, &QAction::triggered, this, &MainWindow::slotZoomUndoScene);
    m_ActionZoomUndoScene->setEnabled(false);

    auto actionSetup = new QAction(QIcon(":/resources/img/setup.svg"), tr("Settings"), this);
    QObject::connect(actionSetup, &QAction::triggered, this, &MainWindow::slotSetup);

    m_ActionStepStop = new QAction(QIcon(":/resources/img/step_stop.svg"), tr("Step / Stop"), this);
    QObject::connect(m_ActionStepStop, &QAction::triggered, this, &MainWindow::slotStepStop);
    m_ActionStepStop->setEnabled(false);
    m_ActionStepStop->setShortcut(Qt::CTRL + Qt::Key_Space);

    m_ActionRun = new QAction(QIcon(":/resources/img/run.svg"), tr("Run"), this);
    QObject::connect(m_ActionRun, &QAction::triggered, this, &MainWindow::slotRun);
    m_ActionRun->setEnabled(false);
    m_ActionRun->setShortcut(Qt::CTRL + Qt::Key_R);

    m_ActionEditCell = new QAction(QIcon(":/resources/img/edit.svg"), tr("Edit cell"), this);
    QObject::connect(m_ActionEditCell, &QAction::triggered, this, &MainWindow::slotEditCell);
    m_ActionEditCell->setEnabled(false);
    m_ActionEditCell->setShortcut(Qt::CTRL + Qt::Key_E);

    m_ActionInfoCell = new QAction(QIcon(":/resources/img/check.svg"), tr("Cell information"), this);
    QObject::connect(m_ActionInfoCell, &QAction::triggered, this, &MainWindow::slotInfoCell);
    m_ActionInfoCell->setEnabled(false);

    m_ActionSaveCellsToClipbord = new QAction(QIcon(":/resources/img/copy.svg"), tr("Copy selected cells to clipboard"), this);
    QObject::connect(m_ActionSaveCellsToClipbord, &QAction::triggered, this, &MainWindow::slotSaveCellsToClipbord);
    m_ActionSaveCellsToClipbord->setEnabled(false);
    m_ActionSaveCellsToClipbord->setShortcut(Qt::CTRL + Qt::Key_C);

    m_ActionLoadCellsFromClipbord = new QAction(QIcon(":/resources/img/paste.svg"), tr("Paste cells from clipboard"), this);
    QObject::connect(m_ActionLoadCellsFromClipbord, &QAction::triggered, this, &MainWindow::slotLoadCellsFromClipbord);
    m_ActionLoadCellsFromClipbord->setEnabled(false);
    m_ActionLoadCellsFromClipbord->setShortcut(Qt::CTRL + Qt::Key_V);

    m_ActionInfoField = new QAction(QIcon(":/resources/img/field.svg"), tr("Field information"), this);
    QObject::connect(m_ActionInfoField, &QAction::triggered, this, &MainWindow::slotInfoField);
    m_ActionInfoField->setEnabled(false);
    m_ActionInfoField->setShortcut(Qt::CTRL + Qt::Key_F);

    m_ActionSaveImageToFile = new QAction(QIcon(":/resources/img/save.svg"), tr("Save image to file"), this);
    QObject::connect(m_ActionSaveImageToFile, &QAction::triggered, this, &MainWindow::slotSaveImageToFile);
    m_ActionSaveImageToFile->setEnabled(false);

    // тулбар
    auto tbMain = new QToolBar(this);
    tbMain->setMovable(false);
    tbMain->setOrientation(Qt::Horizontal);
    tbMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    tbMain->addAction(actionNew);
    tbMain->addSeparator();
    tbMain->addAction(m_ActionZoomUndoScene);
    tbMain->addAction(m_ActionZoomInScene);
    tbMain->addAction(m_ActionZoomOutScene);
    tbMain->addSeparator();
    tbMain->addAction(m_ActionEditCell);
    tbMain->addAction(m_ActionInfoCell);
    tbMain->addAction(m_ActionSaveCellsToClipbord);
    tbMain->addAction(m_ActionLoadCellsFromClipbord);
    tbMain->addSeparator();
    tbMain->addAction(m_ActionInfoField);
    tbMain->addAction(m_ActionSaveImageToFile);
    tbMain->addSeparator();
    tbMain->addAction(m_ActionStepStop);
    tbMain->addAction(m_ActionRun);

    tbMain->addWidget(new WidgetSpacer(this));
    tbMain->addAction(actionSetup);
    tbMain->addSeparator();
    tbMain->addAction(actionQt);
    tbMain->addAction(actionExit);

    addToolBar(tbMain);

    // SceneView
    m_SceneView = new SceneView(this);
    setCentralWidget(m_SceneView);

    // статусбар
    auto statusBar = new QStatusBar(this);

    m_LabelRunningStatus = new QLabel(this);
    slotFieldRunning(false);
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
    m_LabelFieldPause = new QLabel(QString::number(config->SceneCalculatingMinPause()), this);
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

    m_LabelSelectedCell = new QLabel("-", this);
    statusBar->addWidget(m_LabelSelectedCell);

    statusBar->addWidget(new SeparatorV(this));

    m_ProgressBar = new QProgressBar(this);
    m_ProgressBar->setAlignment(Qt::AlignLeft);
    m_ProgressBar->setFixedWidth(4 * config->ButtonSize());
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
    m_ActionSaveCellsToClipbord->setDisabled(true);
    if(m_SceneView->getScene()->getSelectedCell()) m_ActionLoadCellsFromClipbord->setEnabled(true);
    m_SceneView->getScene()->clearMultiSelection();

    if(m_Field->isCalculating()) Q_EMIT signalStopField();
    else
    {
        m_Field->setRuleOn(true);
        m_Field->setCalculatingNonstop(false);
        m_Field->slotStartCalculating();
        m_ThreadField->start();
        setSceneFieldThreadPriority();
    }
}

void MainWindow::slotRun()
{
    if(!m_Field->isCalculating())
    {
        m_ActionSaveCellsToClipbord->setDisabled(true);
        m_ActionLoadCellsFromClipbord->setDisabled(true);
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
    auto scene = m_SceneView->addScene(m_Field);
    scene->addSceneItem();

    QObject::connect(scene, &Scene::signalAverageDrawChangedUp, this, &MainWindow::slotAverageDrawUp);
    QObject::connect(scene, &Scene::signalAverageDrawChangedDown, this, &MainWindow::slotAverageDrawDown);
    QObject::connect(scene, &Scene::signalSelectedCellChanged, this, &MainWindow::slotSelectedCellChanged);
    QObject::connect(scene, &Scene::signalShowCellInfo, this, &MainWindow::slotInfoCell);
    QObject::connect(scene, &Scene::signalSelectedCellsChanged, this, &MainWindow::slotSelectedCellsChanged);
}

void MainWindow::createField(int w, int h, bool random)
{
    auto scene = m_SceneView->getScene();
    if(scene) scene->StopAdvanse();

    deleteField();

    m_ThreadField = new QThread;
    QObject::connect(m_ThreadField, &QObject::destroyed, [=](){ qDebug() << "ThreadField destroyed"; });

    m_Field = new Field(w, h);

    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAgeChanged, this, &MainWindow::slotFieldAge);
    QObject::connect(m_Field, &Field::signalCalculating, this, &MainWindow::slotFieldRunning);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedUp, this, &MainWindow::slotFieldAvCalcUp);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedDown, this, &MainWindow::slotFieldAvCalcDown);
    QObject::connect(this, &MainWindow::signalStopField, m_Field, &Field::slotStopCalculating, Qt::DirectConnection);

    fillField(random);
    m_Field->moveToThread(m_ThreadField);  // NOTE: field выполняется не в основном потоке

    QObject::connect(m_Field, &Field::signalCalculatingDone, this, &MainWindow::stopThreadField, Qt::DirectConnection);
    QObject::connect(m_ThreadField, &QThread::started, m_Field, &Field::calculate, Qt::DirectConnection);
}

void MainWindow::fillField(bool random)
{
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

void MainWindow::setActionsEnable(bool value)
{
    m_ActionZoomInScene->setEnabled(value);
    m_ActionZoomOutScene->setEnabled(value);
    m_ActionZoomUndoScene->setEnabled(value);
    m_ActionStepStop->setEnabled(value);
    m_ActionInfoField->setEnabled(value);
    m_ActionSaveImageToFile->setEnabled(value);
    m_ActionRun->setEnabled(value);
}

void MainWindow::deleteField()
{
    Q_EMIT signalStopField();
    if(m_Field)
    {
        m_Field->AbortCalculating();
        while(m_ThreadField->isRunning()) qDebug() << "Waiting ThreadField";

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
    m_ThreadField->quit();
    m_ThreadField->requestInterruption();
}

void MainWindow::setSceneFieldThreadPriority()
{
    if(!m_ThreadField || !m_ThreadField->isRunning()) return;

    auto mode = config->SceneFieldThreadPriority().toUpper();

    if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(0)) m_ThreadField->setPriority(QThread::LowPriority);
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(1)) m_ThreadField->setPriority(QThread::NormalPriority);
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(2)) m_ThreadField->setPriority(QThread::HighPriority);
    else if(mode == SCENE_FIELD_THREAD_PRIORITIES.at(3)) m_ThreadField->setPriority(QThread::HighestPriority);
    else
    {
        qCritical() << "Wrong settins value 'Scene/FieldThreadPriority'" <<  mode;
        m_ThreadField->setPriority(QThread::NormalPriority);
    }
    qInfo() << "Field thread priority:" << m_ThreadField->priority() << mode;
}

void MainWindow::redrawScene()
{
    m_Field->setRuleOn(false);
    m_Field->setCalculatingNonstop(false);
    m_Field->slotStartCalculating();
    m_Field->calculate();
}

void MainWindow::CellsToJsonObject(QJsonObject* jobject, Cell *firstcell, Cell *secondcell)
{
    auto xmin = qMin(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto xmax = qMax(firstcell->getIndex().x(), secondcell->getIndex().x());
    auto ymin = qMin(firstcell->getIndex().y(), secondcell->getIndex().y());
    auto ymax = qMax(firstcell->getIndex().y(), secondcell->getIndex().y());

    m_ProgressBar->setRange(0, xmax - xmin); // индикация без затей по X
    m_ProgressBar->setValue(0);
    m_ProgressBar->show();

    int dx = 0, dy = 0;
    QJsonArray cells;
    for(int x = xmin; x <= xmax; x++)
    {
        for(int y = ymin; y <= ymax; y++)
        {
            dx = x - xmin; dy = y - ymin;
            auto ci = m_Field->cells()->at(x).at(y)->getCurInfo();
            auto map = getPropertiesList(ci);

            QJsonObject obj_index;
            obj_index.insert("X", dx);
            obj_index.insert("Y", dy);

            QJsonObject obj_prop;
            for(auto key: map.keys())
            {
                QVariant value = ci->property(key.toStdString().c_str());
                obj_prop.insert(key, value.toString());
            }

            QJsonObject obj_cell;
            obj_cell["Index"] = obj_index;
            obj_cell["Properties"] = obj_prop;
            cells.append(obj_cell);
        }
        m_ProgressBar->setValue(dx);
    }
    jobject->insert("Cells", cells);
    qDebug() << "Copied to JsonObject" << cells.count() << "cells";
    m_ProgressBar->hide();
}

void MainWindow::CellsFromJsonObject(QJsonObject *jobject, Cell *cell)
{
    Q_UNUSED(jobject);
    Q_UNUSED(cell);

}

void MainWindow::slotSetup()
{
    const QVector<QString> keys = {tr("00#_Common options"),
                                   tr("01#_Date and time format"),
                                   tr("02#_Use SI metric"),
                                   tr("03#_Event log size (0 = maximum)"),
                                   tr("04#_Buttons size"),
                                   tr("05#_Scene options"),
                                   tr("06#_Scene antialiasing"),
                                   tr("07#_Scene update mode"),
                                   tr("08#_Scene background color"),
                                   tr("09#_Scene selection color"),//
                                   tr("10#_Dead cell color"),
                                   tr("11#_Cursed cell color"),
                                   tr("12#_Scene zoom factor"),
                                   tr("13#_Indicate age value"),
                                   tr("14#_Minimum pause at calculating (ms)"),
                                   tr("15#_Field thread priority"),
                                  };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
     {keys.at(2), {QVariant::Bool, config->SIMetric(), 0, 0}},
     {keys.at(3), {QVariant::Int, config->LogSize(), 0, 0}},
     {keys.at(4), {QVariant::Int, config->ButtonSize(), 16, 100}},
     {keys.at(5), {}},
     {keys.at(6), {QVariant::Bool, config->SceneViewAntialiasing(), 0, 0}},
     {keys.at(7), {QVariant::StringList, config->SceneViewUpdateMode(), 0, SCENE_VIEW_UPDATE_MODES, DialogValueMode::OneFromList}},
     {keys.at(8), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(9), {QVariant::String, config->SceneSelectColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(10), {QVariant::String, config->SceneCellDeadColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(11), {QVariant::String, config->SceneCellCurseColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(12), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
     {keys.at(13), {QVariant::Bool, config->SceneCellAgeIndicate(), 0, 0}},
     {keys.at(14), {QVariant::Int, config->SceneCalculatingMinPause(), 0, 10000}},
     {keys.at(15), {QVariant::StringList, config->SceneFieldThreadPriority(), 1, SCENE_FIELD_THREAD_PRIORITIES, DialogValueMode::OneFromList}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/setup.svg", tr("Settings"), &map);

    if(!dvl->exec()) return;

    // common
    config->setDateTimeFormat(map.value(keys.at(1)).value.toString());
    config->setSIMetric(map.value(keys.at(2)).value.toBool());
    config->setLogSize(map.value(keys.at(3)).value.toInt());
    config->setButtonSize(map.value(keys.at(4)).value.toInt());
    // scene
    config->setSceneViewAntialiasing(map.value(keys.at(6)).value.toBool());
    m_SceneView->setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    config->setSceneViewUpdateMode(map.value(keys.at(7)).value.toString());
    m_SceneView->SetUpdateMode();
    config->setSceneBgColor(map.value(keys.at(8)).value.toString());
    if(m_SceneView->getScene())
    {
        m_SceneView->getScene()->setBackgroundColor(config->SceneBgColor());
        m_SceneView->update();
    }
    config->setSceneSelectColor(map.value(keys.at(9)).value.toString());
    if(m_SceneView->getScene())
    {
        m_SceneView->getScene()->setSelectionMarkColor(config->SceneSelectColor());
        m_SceneView->update();
    }
    config->setSceneCellDeadColor(map.value(keys.at(10)).value.toString());
    config->setSceneCellCurseColor(map.value(keys.at(11)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(12)).value.toDouble());
    config->setSceneCellAgeIndicate(map.value(keys.at(13)).value.toBool());
    config->setSceneCalculatingMinPause(map.value(keys.at(14)).value.toInt());
    m_LabelFieldPause->setText(QString::number(config->SceneCalculatingMinPause()));
    config->setSceneFieldThreadPriority(map.value(keys.at(15)).value.toString());
    setSceneFieldThreadPriority();
}

void MainWindow::slotEditCell()
{
    Q_EMIT signalStopField();

    auto cell = m_SceneView->getScene()->getSelectedCell();
    if(!cell) { m_ActionEditCell->setDisabled(true); return; }

    m_SceneView->getScene()->selectCell(nullptr);
    slotShowCell(cell);

    auto cni = cell->getNewInfo();
    auto statelist = listKernelEnum("CellState");

    const QVector<QString> keys =
    { tr("00#_Cell properties"),
      tr("01#_Cell state"),
      tr("02#_Cell age"),
      tr("03#_Cell generation") };
    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList,
                    getNameKernelEnum("CellState", static_cast<int>(cni->getState())), 0,
                    statelist, DialogValueMode::OneFromList}},
      {keys.at(2), {QVariant::Int, cni->getAge(), 0, 0}},
      {keys.at(3), {QVariant::Int, cni->getGeneration(), 0, 0}} };

    auto dvl = new DialogValuesList(this, ":/resources/img/point.svg",
                                    tr("Edit cell %1").arg(cell->objectName()), &map);
    if(!dvl->exec()) return;

    cni->setState(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(1)).value.toString())));
    cni->setAge(map.value(keys.at(2)).value.toInt());
    cni->setGeneration(map.value(keys.at(3)).value.toInt());

    cell->applyInfo(); // для ускорения обновления; в m_Field->calculate() applyCalculating выключен
    redrawScene();
}

void MainWindow::slotNewProject()
{
    Q_EMIT signalStopField();

    QMap<QString, FieldRule*> ruleslist;
    auto rule = new FieldRule; // default rule
    ruleslist.insert(rule->objectName(), rule);

    //TODO: загружать здесь остальные возможные правила
    const QVector<QString> keys = {
        tr("00#_Field properties"),
        tr("01#_Size"),
        tr("02#_Cell size"),
        tr("03#_Rule [%1]:").arg(QString::number(ruleslist.count())),
        tr("04#_Options"),
        tr("05#_Random filling"),
        tr("06#_Show field information"),
    };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::Int, config->SceneFieldSize(), 2, 10000}},
     {keys.at(2), {QVariant::Int, config->SceneCellSize(), 1, 100}},
     {keys.at(3), {QVariant::StringList, ruleslist.keys().at(0), 0, QStringList(ruleslist.keys()), DialogValueMode::OneFromList}},
     {keys.at(4), {}},
     {keys.at(5), {QVariant::Bool, false}},
     {keys.at(6), {QVariant::Bool, config->WindowShowFieldInfo()}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/asterisk.svg", tr("New project"), &map);

    if(!dvl->exec())
    {
        rule->deleteLater();
        return;
    }

    config->setSceneFieldSize(map.value(keys.at(1)).value.toInt());
    config->setSceneCellSize(map.value(keys.at(2)).value.toInt());
    auto random = map.value(keys.at(5)).value.toBool();
    config->setWindowShowFieldInfo(map.value(keys.at(6)).value.toBool());

    createField(config->SceneFieldSize(), config->SceneFieldSize(), random);

    auto currentrule = map.value(keys.at(3)).value.toString();
    m_Field->setRule(ruleslist.value(currentrule));

    setWindowTitle(QString("%1 %2 <%3> [%4 X %5 X %6]").
                   arg(APP_NAME, APP_VERS, currentrule,
                       QString::number(m_Field->width()),
                       QString::number(m_Field->height()),
                       QString::number(config->SceneCellSize())));

    m_SceneView->zoomer()->Zoom(-1.0);
    m_LabelFieldAvCalc->setText("0 ms");
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

    createScene();

    m_ActionEditCell->setDisabled(true);
    m_ActionInfoCell->setDisabled(true);
    m_ActionSaveCellsToClipbord->setDisabled(true);
    setActionsEnable(true);

    if(random) redrawScene();
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

void MainWindow::slotSelectedCellChanged(Cell *cell)
{
    m_ActionEditCell->setDisabled(cell == nullptr);
    m_ActionInfoCell->setDisabled(cell == nullptr);
    m_ActionLoadCellsFromClipbord->setDisabled(cell == nullptr);
    if(cell) m_LabelSelectedCell->setText(cell->objectName());
    else m_LabelSelectedCell->setText("-");
}

void MainWindow::slotSaveCellsToClipbord()
{
    auto scene = m_SceneView->getScene();
    if(!scene) {m_ActionSaveCellsToClipbord->setDisabled(true); return; }

    auto firstcell = scene->getSelectedCell();
    auto secondcell = scene->getSecondSelectedCell();
    if(!firstcell || !secondcell || firstcell == secondcell)  {m_ActionSaveCellsToClipbord->setDisabled(true); return; }

    QJsonDocument document;
    QJsonObject obj_root;
    obj_root.insert("application", APP_NAME);
    obj_root.insert("version", APP_VERS);

    CellsToJsonObject(&obj_root, firstcell, secondcell);
    document.setObject(obj_root);

    auto clipboard = QGuiApplication::clipboard();
    clipboard->setText(document.toJson(QJsonDocument::Indented));
}

void MainWindow::slotLoadCellsFromClipbord()
{

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

    if(DialogFieldInformation::FindPreviousCopy(m_Field)) return;

    auto dfi = new DialogFieldInformation(this, m_Field);
    dfi->show();
}

void MainWindow::slotShowCell(Cell *cell)
{
    if(!cell)
    {
        qWarning() << __func__ << "Cell is null";
        return;
    }

    m_SceneView->getScene()->selectCell(cell);
    m_SceneView->centerOn(cell->getIndex() * config->SceneCellSize());
}

void MainWindow::slotSaveImageToFile()
{
    if(!m_Field) { m_ActionSaveImageToFile->setDisabled(true); return; }

    auto fileformat = config->SceneImageFileFormat().toLower();
    auto filename = QFileDialog::getSaveFileName(this, "Save image", config->LastDir(),
                                                 tr("%1 files (*.%2)").arg(fileformat.toUpper(), fileformat));

    if(filename.isNull()) return;

    config->setLastDir(QFileInfo(filename).dir().path());

    auto dot_fileformat = QString(".%1").arg(fileformat);
    if(!filename.endsWith(dot_fileformat, Qt::CaseInsensitive)) filename.append(dot_fileformat);

    auto pixmap = m_SceneView->getScene()->getSceneItem()->getPixmap();
    if(!pixmap->save(filename, fileformat.toUpper().toLatin1().constData()))
        QMessageBox::critical(this, tr("Error"),
                              tr("Error at file saving. Path: '%1'").arg(filename));
}

void MainWindow::slotSelectedCellsChanged(Cell *first, Cell *second)
{
    if(!first || !second || first == second) { m_ActionSaveCellsToClipbord->setDisabled(true); return; }

    Q_EMIT signalStopField();
    m_ActionSaveCellsToClipbord->setEnabled(true);

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

void MainWindow::slotFieldAge(qint64 value) { m_LabelFieldAge->setText(QString::number(value)); }
QProgressBar *MainWindow::ProgressBar() const { return m_ProgressBar; }
