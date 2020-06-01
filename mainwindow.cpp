#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"
#include "scene/scene.h"
#include "scene/sceneobject.h"
#include "scene/sceneview.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogvalueslist.h"
#include "field/field.h"
#include "field/cell.h"

#include <QDebug>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_Field(nullptr)
{
    setWindowIcon(QIcon(":/resources/img/mainicon512.png"));
    setWindowTitle(QString("%1 %2").arg(APP_NAME, APP_VERS));
    loadGui();
    setWidgetToScreenCenter(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height",height());
    settings.setValue("MainWindow/Width",width());

    m_SceneView->hide(); //
    if(m_SceneView->scene()) m_SceneView->scene()->clear(); //

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
    QObject::connect(actionNew, &QAction::triggered, this, &MainWindow::newProject);

    m_ActionZoomInScene = new QAction(QIcon(":/resources/img/zoom_in.svg"), tr("Zoom IN"), this);
    QObject::connect(m_ActionZoomInScene, &QAction::triggered, this, &MainWindow::slotSceneZoomIn);

    m_ActionZoomOutScene = new QAction(QIcon(":/resources/img/zoom_out.svg"), tr("Zoom OUT"), this);
    QObject::connect(m_ActionZoomOutScene, &QAction::triggered, this, &MainWindow::slotSceneZoomOut);

    m_ActionZoomUndoScene = new QAction(QIcon(":/resources/img/zoom_undo.svg"), tr("Zoom UNDO"), this);
    QObject::connect(m_ActionZoomUndoScene, &QAction::triggered, [=](){ m_SceneView->Zoomer()->Zoom(ZOOM_FACTOR_RESET); });

    auto actionSetup = new QAction(QIcon(":/resources/img/setup.svg"), tr("Settings"), this);
    QObject::connect(actionSetup, &QAction::triggered, this, &MainWindow::slotSetup);

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
    m_LabelFieldSize = new QLabel(this);
    m_LabelFieldSize->setText("-");
    statusBar->addWidget(m_LabelFieldSize);

    statusBar->addWidget(new SeparatorV(this)); // TODO: statusBar separator

    statusBar->addWidget(new QLabel(tr("Selected:"), this));
    m_LabelFocusedObject = new QLabel(this);
    m_LabelFocusedObject->setText("-");
    statusBar->addWidget(m_LabelFocusedObject);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Zoom:"), this));
    m_LabelFieldZoom = new QLabel(this);
    m_LabelFieldZoom->setText("1");
    QObject::connect(m_SceneView->Zoomer(), &GraphicsViewZoomer::signalZoomed,
                     [=](qreal value){ m_LabelFieldZoom->setText(QString::number(value)); });
    statusBar->addWidget(m_LabelFieldZoom);

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

void MainWindow::newProject()
{
    const QVector<QString> keys = {tr("00#_Field options"),
                                   tr("01#_Size"),
                                   tr("02#_Cell size")};
    QMap<QString, DialogValue> map =
        {{keys.at(0), {}},
         {keys.at(1), {QVariant::Int, config->SceneSize(), 2, 10000}},
         {keys.at(2), {QVariant::Int, config->SceneObjectSize(), 1, 100}}
        };

    auto dvl = new DialogValuesList(this, ":/resources/img/asterisk.svg", tr("New project"), &map);

    if(!dvl->exec()) return;

    config->setSceneSize(map.value(keys.at(1)).value.toInt());
    config->setSceneObjectSize(map.value(keys.at(2)).value.toInt());

    createField(config->SceneSize(), config->SceneSize());
    m_SceneView->Zoomer()->Zoom(-1.0);
    createScene();
}

void MainWindow::createScene()
{
    m_SceneView->AddScene(m_Field);

    if(config->ScenePreFill())
    {
        m_ProgressBar->setRange(0, m_Field->Height());
        m_ProgressBar->setValue(0);
        m_ProgressBar->show();

        auto conn = std::make_shared<QMetaObject::Connection>();
        auto func = [=](int step)
        {
            if(step < m_Field->Height()) m_ProgressBar->setValue(step);
            else
            {
                m_ProgressBar->hide();
                qDebug() << "Scene filling disconnection:" << QObject::disconnect(*conn);
            }
        };
        *conn = QObject::connect(m_SceneView->getScene(), &Scene::signalProgress, func);

        m_SceneView->Fill();
    }

    QObject::connect(m_SceneView->getScene(), &QGraphicsScene::focusItemChanged,
                     [=]()
                     {
                         auto o = m_SceneView->getScene()->FocusedObject();
                         if(o) m_LabelFocusedObject->setText(o->getCell()->objectName());
                         else m_LabelFocusedObject->setText("-");
                     });
    m_LabelFieldSize->setText(QString("%1X%2 [%3]").arg(QString::number(m_Field->Width()),
                                                        QString::number(m_Field->Height()),
                                                        QString::number(config->SceneObjectSize())));
}

void MainWindow::createField(int w, int h)
{
    if(m_Field) m_Field->deleteLater();
    m_Field = new Field(this, w, h);
}

    void MainWindow::slotSetup()
{
    const QVector<QString> keys = {tr("00#_Common options"),
        tr("01#_Date and time format"),
        tr("02#_Use SI metric"),
        tr("03#_Event log size (0 = maximum)"),
        tr("04#_Buttons size"),
        tr("05#_Scene options"),
        tr("06#_Scene BspTree indexing"),
        tr("07#_Scene antialiasing"),
        tr("08#_Scene background color"),
        tr("09#_Scene border color"),
        tr("10#_Scene color"),
        tr("11#_Scene zoom factor"),
        tr("12#_Prefill scene"),
        tr("13#_Scene object options"),
        tr("14#_Indicate age value"),
        tr("15#_Dead cell color"),
        tr("16#_Alive cell color (age = 0)"),
        tr("17#_Alive cell color, age = 1"),
        tr("18#_Alive cell color, age = 2"),
        tr("19#_Alive cell color, age = 3"),
        tr("20#_Alive cell color, age = 4"),
        tr("21#_Alive cell color, age = 5"),
        tr("22#_Alive cell color, age = 6"),
        tr("23#_Alive cell color, age = 7"),
        tr("24#_Alive cell color, age = 8"),
        tr("25#_Alive cell color, age >= 9"),
        };
    QMap<QString, DialogValue> map =
        {{keys.at(0), {}},
         {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
         {keys.at(2), {QVariant::Bool, config->SIMetric(), 0, 0}},
         {keys.at(3), {QVariant::Int, config->LogSize(), 0, 0}},
         {keys.at(4), {QVariant::Int, config->ButtonSize(), 16, 100}},
         {keys.at(5), {}},
         {keys.at(6), {QVariant::Bool, config->SceneBspTreeIndex(), 0, 0}},
         {keys.at(7), {QVariant::Bool, config->SceneViewAntialiasing(), 0, 0}},
         {keys.at(8), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
         {keys.at(9), {QVariant::String, config->SceneBorderColor(), 0, 0, DialogValueMode::Color}},
         {keys.at(10), {QVariant::String, config->SceneColor(), 0, 0, DialogValueMode::Color}},
         {keys.at(11), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
         {keys.at(12), {QVariant::Bool, config->ScenePreFill(), 0, 0}},
         {keys.at(13), {}},
         {keys.at(14), {QVariant::Bool, config->SceneObjectAgeIndicate(), 0, 0}},
         {keys.at(15), {QVariant::String, config->SceneObjectDeadColor(), 0, 0, DialogValueMode::Color}},
         {keys.at(16), {QVariant::String, config->SceneObjectAlive0Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(17), {QVariant::String, config->SceneObjectAlive1Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(18), {QVariant::String, config->SceneObjectAlive2Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(19), {QVariant::String, config->SceneObjectAlive3Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(20), {QVariant::String, config->SceneObjectAlive4Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(21), {QVariant::String, config->SceneObjectAlive5Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(22), {QVariant::String, config->SceneObjectAlive6Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(23), {QVariant::String, config->SceneObjectAlive7Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(24), {QVariant::String, config->SceneObjectAlive8Color(), 0, 0, DialogValueMode::Color}},
         {keys.at(25), {QVariant::String, config->SceneObjectAlive9Color(), 0, 0, DialogValueMode::Color}},
         };

    auto dvl = new DialogValuesList(this, ":/resources/img/setup.svg", tr("Settings"), &map);

    if(!dvl->exec()) return;

    // common
    config->setDateTimeFormat(map.value(keys.at(1)).value.toString());
    config->setSIMetric(map.value(keys.at(2)).value.toBool());
    config->setLogSize(map.value(keys.at(3)).value.toInt());
    config->setButtonSize(map.value(keys.at(4)).value.toInt());
    // scene
    config->setSceneBspTreeIndex(map.value(keys.at(6)).value.toBool());
    config->setSceneViewAntialiasing(map.value(keys.at(7)).value.toBool());
    config->setSceneBgColor(map.value(keys.at(8)).value.toString());
    config->setSceneBorderColor(map.value(keys.at(9)).value.toString());
    config->setSceneColor(map.value(keys.at(10)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(11)).value.toDouble());
    config->setScenePreFill(map.value(keys.at(12)).value.toDouble());
    // scene object
    config->setSceneObjectAgeIndicate(map.value(keys.at(14)).value.toBool());
    config->setSceneObjectDeadColor(map.value(keys.at(15)).value.toString());
    config->setSceneObjectAlive0Color(map.value(keys.at(16)).value.toString());
    config->setSceneObjectAlive1Color(map.value(keys.at(17)).value.toString());
    config->setSceneObjectAlive2Color(map.value(keys.at(18)).value.toString());
    config->setSceneObjectAlive3Color(map.value(keys.at(19)).value.toString());
    config->setSceneObjectAlive4Color(map.value(keys.at(20)).value.toString());
    config->setSceneObjectAlive5Color(map.value(keys.at(21)).value.toString());
    config->setSceneObjectAlive6Color(map.value(keys.at(22)).value.toString());
    config->setSceneObjectAlive7Color(map.value(keys.at(23)).value.toString());
    config->setSceneObjectAlive8Color(map.value(keys.at(24)).value.toString());
    config->setSceneObjectAlive9Color(map.value(keys.at(25)).value.toString());
}

void MainWindow::slotSceneZoomIn()
{
    auto factor = config->SceneScaleStep() + 100 * (config->SceneScaleStep() - 1);
    auto scene = m_SceneView->getScene();
    m_SceneView->Zoomer()->Zoom(factor, true);
    if(scene && scene->FocusedObject())
        m_SceneView->centerOn(scene->FocusedObject());
}

void MainWindow::slotSceneZoomOut()
{
    auto factor = 1 / (config->SceneScaleStep() + 100 * (config->SceneScaleStep() - 1));
    auto scene = m_SceneView->getScene();
    m_SceneView->Zoomer()->Zoom(factor, true);
    if(scene && scene->FocusedObject())
        m_SceneView->centerOn(scene->FocusedObject());
}

QProgressBar *MainWindow::ProgressBar() const { return m_ProgressBar; }
