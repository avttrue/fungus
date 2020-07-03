#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"
#include "scene/scene.h"
#include "scene/sceneItem.h"
#include "scene/sceneview.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogvalueslist.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellrule.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ThreadField(nullptr),
      m_Field(nullptr)
{    
    setWindowIcon(QIcon(":/resources/img/flora.svg"));
    setWindowTitle(QString("%1 %2 < >").arg(APP_NAME, APP_VERS));
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

    m_ActionRun = new QAction(QIcon(":/resources/img/run.svg"), tr("Run"), this);
    QObject::connect(m_ActionRun, &QAction::triggered, this, &MainWindow::slotRun);
    m_ActionRun->setEnabled(false);

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
    statusBar->addWidget(new QLabel(tr("Size:"), this));
    m_LabelFieldSize = new QLabel("-", this);
    statusBar->addWidget(m_LabelFieldSize);

    statusBar->addWidget(new SeparatorV(this)); // TODO: statusBar separator

    statusBar->addWidget(new QLabel(tr("Selected:"), this));
    m_LabelFocusedObject = new QLabel("-", this);
    statusBar->addWidget(m_LabelFocusedObject);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Zoom:"), this));
    m_LabelFieldZoom = new QLabel("1", this);
    QObject::connect(m_SceneView->zoomer(), &GraphicsViewZoomer::signalZoomed,
                     [=](qreal value){ m_LabelFieldZoom->setText(QString::number(value)); });
    statusBar->addWidget(m_LabelFieldZoom);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Age:"), this));
    m_LabelFieldAge = new QLabel("-", this);
    statusBar->addWidget(m_LabelFieldAge);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Calc time:"), this));
    m_LabelFieldAvCalc = new QLabel("-", this);
    statusBar->addWidget(m_LabelFieldAvCalc);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Draw time:"), this));
    m_LabelSceneAvDraw = new QLabel("-", this);
    statusBar->addWidget(m_LabelSceneAvDraw);

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

void MainWindow::slotNewProject()
{
    if(m_Field) m_Field->setRunning(false);

    QMap<QString, CellRule*> ruleslist;
    auto rule = new CellRule; // default rule
    ruleslist.insert(rule->objectName(), rule);

    //TODO: загружать здесь остальные возможные правила
    const QVector<QString> keys = {tr("00#_Field properties"),
                                   tr("01#_Size"),
                                   tr("02#_Cell size"),
                                   tr("03#_Rule (%1):").arg(QString::number(ruleslist.count()))};
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::Int, config->SceneFieldSize(), 2, 10000}},
     {keys.at(2), {QVariant::Int, config->SceneCellSize(), 1, 100}},
     {keys.at(3), {QVariant::StringList, ruleslist.keys().at(0), 0, QStringList(ruleslist.keys()), DialogValueMode::OneFromList}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/asterisk.svg", tr("New project"), &map);

    if(!dvl->exec())
    {
        rule->deleteLater();
        return;
    }

    config->setSceneFieldSize(map.value(keys.at(1)).value.toInt());
    config->setSceneCellSize(map.value(keys.at(2)).value.toInt());

    createField(config->SceneFieldSize(), config->SceneFieldSize());
    auto currentrule = map.value(keys.at(3)).value.toString();                  // set rule
    m_Field->setRule(ruleslist.value(currentrule));                             //
    setWindowTitle(QString("%1 %2 <%3>").arg(APP_NAME, APP_VERS, currentrule)); // show rule in caption

    m_SceneView->zoomer()->Zoom(-1.0);
    createScene();
    setActionsEnable(true);
}

void MainWindow::slotStepStop()
{
    if(m_Field->isRunning())
    {
        m_Field->setRunning(false);
    }
    else
    {
        m_Field->setRunningAlways(false);
        m_Field->setRunning(true);
        m_ThreadField->start();
    }
}

void MainWindow::slotRun()
{
    if(!m_Field->isRunning())
    {
        m_Field->setRunningAlways(true);
        m_Field->setRunning(true);
        m_ThreadField->start();
    }
}

void MainWindow::createScene()
{
    auto scene = m_SceneView->addScene(m_Field);
    scene->addSceneItem();

    m_LabelFieldSize->setText(QString("%1X%2 [%3]").arg(QString::number(m_Field->width()),
                                                        QString::number(m_Field->height()),
                                                        QString::number(config->SceneCellSize())));

    m_LabelSceneAvDraw->setText(tr("0 ms"));
    QObject::connect(scene, &Scene::signalAverageDrawChanged, this, &MainWindow::slotAverageDraw);
}

void MainWindow::createField(int w, int h)
{
    auto scene = m_SceneView->getScene();
    if(scene) scene->StopAdvanse();

    deleteField();

    m_ThreadField = new QThread;
    QObject::connect(m_ThreadField, &QObject::destroyed, [=](){ qDebug() << "ThreadField destroyed"; });

    m_Field = new Field(w, h);
    fillField();

    QObject::connect(m_ThreadField, &QThread::started, m_Field, &Field::calculate, Qt::DirectConnection);
    QObject::connect(m_Field, &Field::signalCalculatingStopped, this, &MainWindow::stopThreadField, Qt::DirectConnection);
    m_Field->moveToThread(m_ThreadField);  // NOTE: field выполняется не в основном потоке

    m_LabelFieldAge->setText("0");
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAgeChanged, this, &MainWindow::slotFieldAge);
    m_LabelFieldAvCalc->setText(tr("0 ms"));
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChanged, this, &MainWindow::slotFieldAvCalc);
}

void MainWindow::fillField()
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

    m_Field->fill();
}

void MainWindow::setActionsEnable(bool value)
{
    m_ActionZoomInScene->setEnabled(value);
    m_ActionZoomOutScene->setEnabled(value);
    m_ActionZoomUndoScene->setEnabled(value);
    m_ActionStepStop->setEnabled(value);
    m_ActionRun->setEnabled(value);
}

void MainWindow::deleteField()
{
    if(m_Field)
    {
        m_Field->setRunning(false);
        m_Field->StopCalculating();
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

void MainWindow::slotSetup()
{
    const QVector<QString> keys = {tr("00#_Common options"),
                                   tr("01#_Date and time format"),
                                   tr("02#_Use SI metric"),
                                   tr("03#_Event log size (0 = maximum)"),
                                   tr("04#_Buttons size"),
                                   tr("05#_Scene options"),
                                   tr("06#_Scene antialiasing"),
                                   tr("07#_Scene background color"),
                                   tr("08#_Scene selection color"),//
                                   tr("09#_Dead cell color"),
                                   tr("10#_Cursed cell color"),
                                   tr("11#_Scene zoom factor"),
                                   tr("12#_Indicate age value"),
                                   tr("13#_Minimum pause at calculating (ms)"),
                                  };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
     {keys.at(2), {QVariant::Bool, config->SIMetric(), 0, 0}},
     {keys.at(3), {QVariant::Int, config->LogSize(), 0, 0}},
     {keys.at(4), {QVariant::Int, config->ButtonSize(), 16, 100}},
     {keys.at(5), {}},
     {keys.at(6), {QVariant::Bool, config->SceneViewAntialiasing(), 0, 0}},
     {keys.at(7), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(8), {QVariant::String, config->SceneSelectColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(9), {QVariant::String, config->SceneCellDeadColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(10), {QVariant::String, config->SceneCellCurseColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(11), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
     {keys.at(12), {QVariant::Bool, config->SceneObjectAgeIndicate(), 0, 0}},
     {keys.at(13), {QVariant::Int, config->SceneCalculatingMinPause(), 0, 10000}},
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
    config->setSceneBgColor(map.value(keys.at(7)).value.toString());
    config->setSceneSelectColor(map.value(keys.at(8)).value.toString());
    config->setSceneCellDeadColor(map.value(keys.at(9)).value.toString());
    config->setSceneCellCurseColor(map.value(keys.at(10)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(11)).value.toDouble());
    config->setSceneObjectAgeIndicate(map.value(keys.at(12)).value.toBool());
    config->setSceneCalculatingMinPause(map.value(keys.at(13)).value.toInt());
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

void MainWindow::slotFieldAvCalc(qreal value) { m_LabelFieldAvCalc->setText(tr("%1 ms").arg(QString::number(value, 'f', 1))); }
void MainWindow::slotAverageDraw(qreal value) { m_LabelSceneAvDraw->setText(tr("%1 ms").arg(QString::number(value, 'f', 1))); }
void MainWindow::slotFieldAge(qint64 value) { m_LabelFieldAge->setText(QString::number(value)); }
QProgressBar *MainWindow::ProgressBar() const { return m_ProgressBar; }
