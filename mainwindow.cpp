#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"
#include "scene/scene.h"
#include "scene/sceneItem.h"
#include "scene/sceneview.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogvalueslist.h"
#include "dialogs/dialogcellinformation.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellinformation.h"
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

    m_ActionInfoCell = new QAction(QIcon(":/resources/img/eye.svg"), tr("Cell information"), this);
    QObject::connect(m_ActionInfoCell, &QAction::triggered, this, &MainWindow::slotInfoCell);
    m_ActionInfoCell->setEnabled(false);
    m_ActionInfoCell->setShortcut(Qt::CTRL + Qt::Key_I);

    m_ActionShowSelectedCell = new QAction(QIcon(":/resources/img/point.svg"), tr("Show selected cell"), this);
    QObject::connect(m_ActionShowSelectedCell, &QAction::triggered, this, &MainWindow::slotShowSelectedCell);
    m_ActionShowSelectedCell->setEnabled(false);
    m_ActionShowSelectedCell->setShortcut(Qt::CTRL + Qt::Key_S);

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
    tbMain->addAction(m_ActionShowSelectedCell);
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

    statusBar->addWidget(new QLabel(tr("Zoom:"), this));
    m_LabelFieldZoom = new QLabel("1", this);
    QObject::connect(m_SceneView->zoomer(), &GraphicsViewZoomer::signalZoomed,
                     [=](qreal value){ m_LabelFieldZoom->setText(QString::number(value)); });
    statusBar->addWidget(m_LabelFieldZoom);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("Age:"), this));
    m_LabelFieldAge = new QLabel("-", this);
    m_LabelFieldAge->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldAge);

    statusBar->addWidget(new SeparatorV(this));

    statusBar->addWidget(new QLabel(tr("D:"), this));
    m_LabelFieldDeadCells = new QLabel("-", this);
    m_LabelFieldDeadCells->setToolTip(tr("Dead cells"));
    m_LabelFieldDeadCells->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldDeadCells);

    statusBar->addWidget(new QLabel(tr("A:"), this));
    m_LabelFieldAliveCells = new QLabel("-", this);
    m_LabelFieldAliveCells->setToolTip(tr("Alive cells"));
    m_LabelFieldAliveCells->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldAliveCells);

    statusBar->addWidget(new QLabel(tr("C:"), this));
    m_LabelFieldCursedCells = new QLabel("-", this);
    m_LabelFieldCursedCells->setToolTip(tr("Cursed cells"));
    m_LabelFieldCursedCells->setStyleSheet(MW_LABEL_STYLE);
    statusBar->addWidget(m_LabelFieldCursedCells);

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

    statusBar->addWidget(new QLabel(tr("Sel:"), this));
    m_LabelSelectedCell = new QLabel("-", this);
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
                                   tr("03#_Rule [%1]:").arg(QString::number(ruleslist.count()))};
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
    auto currentrule = map.value(keys.at(3)).value.toString();
    m_Field->setRule(ruleslist.value(currentrule));

    setWindowTitle(QString("%1 %2 <%3> [%4 X %5 X %6]").
                   arg(APP_NAME, APP_VERS, currentrule,
                       QString::number(m_Field->width()),
                       QString::number(m_Field->height()),
                       QString::number(config->SceneCellSize())));

    m_SceneView->zoomer()->Zoom(-1.0);
    m_ActionEditCell->setDisabled(true);
    m_ActionInfoCell->setDisabled(true);
    m_ActionShowSelectedCell->setDisabled(true);
    m_LabelSceneAvDraw->setText(tr("0 ms"));
    m_LabelSelectedCell->setText("-");

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
        m_Field->setRuleOn(true);
        m_Field->setRunningAlways(false);
        m_Field->setRunning(true);
        m_ThreadField->start();
    }
}

void MainWindow::slotRun()
{
    if(!m_Field->isRunning())
    {
        m_Field->setRuleOn(true);
        m_Field->setRunningAlways(true);
        m_Field->setRunning(true);
        m_ThreadField->start();
    }
}

void MainWindow::createScene()
{
    auto scene = m_SceneView->addScene(m_Field);
    scene->addSceneItem();

    QObject::connect(scene, &Scene::signalAverageDrawChangedUp, this, &MainWindow::slotAverageDrawUp);
    QObject::connect(scene, &Scene::signalAverageDrawChangedDown, this, &MainWindow::slotAverageDrawDown);
    QObject::connect(scene, &Scene::signalSelectedCellChanged, this, &MainWindow::slotSelectedCellChanged);
}

void MainWindow::createField(int w, int h)
{
    auto scene = m_SceneView->getScene();
    if(scene) scene->StopAdvanse();

    deleteField();

    m_ThreadField = new QThread;
    QObject::connect(m_ThreadField, &QObject::destroyed, [=](){ qDebug() << "ThreadField destroyed"; });

    m_Field = new Field(w, h);

    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAgeChanged, this, &MainWindow::slotFieldAge);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalDeadCellsChanged, this, &MainWindow::slotFieldDeadCells);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAliveCellsChanged, this, &MainWindow::slotFieldAliveCells);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signaCursedCellsChanged, this, &MainWindow::slotFieldCursedCells);

    m_LabelFieldAvCalc->setText(tr("0 ms"));
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedUp, this, &MainWindow::slotFieldAvCalcUp);
    QObject::connect(m_Field->getInformation(), &FieldInformation::signalAverageCalcChangedDown, this, &MainWindow::slotFieldAvCalcDown);

    fillField();

    QObject::connect(m_ThreadField, &QThread::started, m_Field, &Field::calculate, Qt::DirectConnection);
    QObject::connect(m_Field, &Field::signalCalculatingStopped, this, &MainWindow::stopThreadField, Qt::DirectConnection);
    m_Field->moveToThread(m_ThreadField);  // NOTE: field выполняется не в основном потоке
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
                                   tr("07#_Scene update mode"),
                                   tr("08#_Scene background color"),
                                   tr("09#_Scene selection color"),//
                                   tr("10#_Dead cell color"),
                                   tr("11#_Cursed cell color"),
                                   tr("12#_Scene zoom factor"),
                                   tr("13#_Indicate age value"),
                                   tr("14#_Minimum pause at calculating (ms)"),
                                  };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String, config->DateTimeFormat(), 0, 0}},
     {keys.at(2), {QVariant::Bool, config->SIMetric(), 0, 0}},
     {keys.at(3), {QVariant::Int, config->LogSize(), 0, 0}},
     {keys.at(4), {QVariant::Int, config->ButtonSize(), 16, 100}},
     {keys.at(5), {}},
     {keys.at(6), {QVariant::Bool, config->SceneViewAntialiasing(), 0, 0}},
     {keys.at(7), {QVariant::StringList, config->SceneViewUpdateMode(), 0, VIEW_UPDATE_MODE, DialogValueMode::OneFromList}},
     {keys.at(8), {QVariant::String, config->SceneBgColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(9), {QVariant::String, config->SceneSelectColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(10), {QVariant::String, config->SceneCellDeadColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(11), {QVariant::String, config->SceneCellCurseColor(), 0, 0, DialogValueMode::Color}},
     {keys.at(12), {QVariant::Double, config->SceneScaleStep(), 1.0, 10.0}},
     {keys.at(13), {QVariant::Bool, config->SceneObjectAgeIndicate(), 0, 0}},
     {keys.at(14), {QVariant::Int, config->SceneCalculatingMinPause(), 0, 10000}},
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
    config->setSceneSelectColor(map.value(keys.at(9)).value.toString());
    config->setSceneCellDeadColor(map.value(keys.at(10)).value.toString());
    config->setSceneCellCurseColor(map.value(keys.at(11)).value.toString());
    config->setSceneScaleStep(map.value(keys.at(12)).value.toDouble());
    config->setSceneObjectAgeIndicate(map.value(keys.at(13)).value.toBool());
    config->setSceneCalculatingMinPause(map.value(keys.at(14)).value.toInt());
    m_LabelFieldPause->setText(tr("%1 ms").arg(QString::number(config->SceneCalculatingMinPause())));
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

void MainWindow::slotFieldDeadCells(qint64 value)
{
    if(!m_Field) return;

    auto num =  QString::number(m_Field->getCellsCount()).length();
    m_LabelFieldDeadCells->setText(QString::number(value).rightJustified(num, '.'));
}

void MainWindow::slotFieldAliveCells(qint64 value)
{
    if(!m_Field) return;

    auto num =  QString::number(m_Field->getCellsCount()).length();
    m_LabelFieldAliveCells->setText(QString::number(value).rightJustified(num, '.'));
}

void MainWindow::slotFieldCursedCells(qint64 value)
{
    if(!m_Field) return;

    auto num =  QString::number(m_Field->getCellsCount()).length();
    m_LabelFieldCursedCells->setText(QString::number(value).rightJustified(num, '.'));
}

void MainWindow::slotSelectedCellChanged(Cell *cell)
{
    m_ActionEditCell->setDisabled(cell == nullptr);
    m_ActionInfoCell->setDisabled(cell == nullptr);
    m_ActionShowSelectedCell->setDisabled(cell == nullptr);

    if(cell) m_LabelSelectedCell->setText(cell->objectName());
    else m_LabelSelectedCell->setText("-");
}

void MainWindow::slotInfoCell()
{
    auto cell = m_SceneView->getScene()->getSelectedCell();
    if(!cell)
    {
        m_ActionInfoCell->setDisabled(true);
        return;
    }

    slotShowCell(cell);

    if(DialogCellInformation::FindPreviousCopy(cell)) return;

    auto dci = new DialogCellInformation(this, cell);

    QObject::connect(dci, &DialogCellInformation::signalShowCell, this, &MainWindow::slotShowCell);
    dci->show();
}

void MainWindow::slotEditCell()
{
    m_Field->setRunning(false);

    auto cell = m_SceneView->getScene()->getSelectedCell();
    if(!cell)
    {
        m_ActionEditCell->setDisabled(true);
        return;
    }

    slotShowCell(cell);

    auto cellinfo = cell->getNewInfo();
    auto statelist = listKernelEnum("CellState");

    const QVector<QString> keys =
    { tr("00#_Cell properties"),
      tr("01#_Cell state"),
      tr("02#_Cell age"),
      tr("03#_Cell generation") };
    QMap<QString, DialogValue> map =
    { {keys.at(0), {}},
      {keys.at(1), {QVariant::StringList,
                    getNameKernelEnum("CellState", static_cast<int>(cellinfo->getState())), 0,
                    statelist, DialogValueMode::OneFromList}},
      {keys.at(2), {QVariant::Int, cellinfo->getAge(), 0, 0}},
      {keys.at(3), {QVariant::Int, cellinfo->getGeneration(), 0, 0}} };

    auto dvl = new DialogValuesList(this, ":/resources/img/point.svg",
                                    tr("Edit cell %1").arg(cell->objectName()), &map);
    if(!dvl->exec()) return;

    cellinfo->setState(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(1)).value.toString())));
    cellinfo->setAge(map.value(keys.at(2)).value.toInt());
    cellinfo->setGeneration(map.value(keys.at(3)).value.toInt());

    m_Field->setRuleOn(false);
    m_Field->setRunningAlways(false);
    m_Field->setRunning(true);
    m_Field->calculate();
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

void MainWindow::slotShowSelectedCell()
{
    auto cell = m_SceneView->getScene()->getSelectedCell();
    if(!cell)
    {
        m_ActionShowSelectedCell->setDisabled(true);
        return;
    }

    slotShowCell(cell);
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
