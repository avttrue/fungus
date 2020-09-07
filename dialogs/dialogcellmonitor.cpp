#include "dialogcellmonitor.h"
#include "properties.h"
#include "controls.h"
#include "textlog.h"
#include "scene/scene.h"
#include "field/field.h"
#include "field/cell.h"

#include <QDebug>
#include <QToolBar>
#include <QVBoxLayout>

DialogCellMonitor::DialogCellMonitor(QWidget *parent, Scene *scene, const QString& title)
    :QDialog(parent),
      m_Scene(scene),
      m_CurrentCellIndex(-1)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(title);
    setWindowIcon(QIcon(":/resources/img/cell_monitor.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(2);
    vblForm->setSpacing(2);
    setLayout(vblForm);

    auto toolBarMain = new QToolBar();
    toolBarMain->setOrientation(Qt::Horizontal);
    toolBarMain->setMovable(false);
    toolBarMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto actionClose = new QAction(QIcon(":/resources/img/no.svg"), tr("Close"));
    QObject::connect(actionClose, &QAction::triggered, [=](){ close(); });
    actionClose->setAutoRepeat(false);

    auto actionClear = new QAction(QIcon(":/resources/img/delete.svg"), tr("Clear observation list"));
    QObject::connect(actionClear, &QAction::triggered, this, &DialogCellMonitor::slotClearObservationList);
    actionClear->setAutoRepeat(false);

    m_ActionSelectCell = new QAction(QIcon(":/resources/img/check.svg"), tr("Add cell to observation list"));
    QObject::connect(m_ActionSelectCell, &QAction::triggered, this, &DialogCellMonitor::slotAddCell);
    m_ActionSelectCell->setAutoRepeat(false);
    m_ActionSelectCell->setEnabled(false);

    m_ActionNextCell = new QAction(QIcon(":/resources/img/right_arrow.svg"), tr("Show next cell"));
    QObject::connect(m_ActionNextCell, &QAction::triggered, this, &DialogCellMonitor::slotShowNextCell);
    m_ActionNextCell->setAutoRepeat(false);
    m_ActionNextCell->setEnabled(false);

    m_LabelCount = new QLabel("0", this);

    toolBarMain->addAction(m_ActionSelectCell);
    toolBarMain->addAction(m_ActionNextCell);
    toolBarMain->addSeparator();
    toolBarMain->addAction(actionClear);
    toolBarMain->addSeparator();
    toolBarMain->addWidget(new QLabel("Observation list: ", this));
    toolBarMain->addWidget(m_LabelCount);
    toolBarMain->addWidget(new WidgetSpacer());
    toolBarMain->addAction(actionClose);

    m_TextContent = new TextLog(this);
    slotAddText(tr("Select a cell and push selection button"));

    vblForm->addWidget(m_TextContent);
    vblForm->addWidget(toolBarMain);

    installEventFilter(this);
    resize(config->CellMonitorWindowWidth(), config->CellMonitorWindowHeight());

    slotSelectedCellChanged(m_Scene->getSelectedCell());
    QObject::connect(m_Scene, &Scene::signalSelectedCellChanged, this, &DialogCellMonitor::slotSelectedCellChanged);
    QObject::connect(m_Scene->getField(), &Field::signalRuleMessage, this, &DialogCellMonitor::slotAddText);
    QObject::connect(m_Scene, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection);
    QObject::connect(this, &QObject::destroyed,
                     [=]() { qDebug() << "DialogCellMonitor" << windowTitle() << "destroyed"; });
    qDebug() << "DialogCellMonitor" << windowTitle() << "created";
}

bool DialogCellMonitor::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowStateChange:
    {
        if(windowState() == Qt::WindowMinimized ||
                windowState() == Qt::WindowMaximized)
        {
            setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
            return true;
        }
        return false;
    }
    case QEvent::Hide:
    case QEvent::Close:
    {
        if(object != this || isMinimized() || isMaximized()) return false;

        config->setCellMonitorWindowWidth(width());
        config->setCellMonitorWindowHeight(height());

        return true;
    }
    default: { return false; }
    }
}

void DialogCellMonitor::addStartText()
{
    slotAddText(tr("Select a cell and push selection button"));
}

void DialogCellMonitor::slotAddCell()
{
    auto cell = m_Scene->getSelectedCell();
    if(!cell) return;
    if(m_Cells.contains(cell)) return;

    if(m_Cells.isEmpty()) m_TextContent->clear();

    m_TextContent->addTextSeparator();
    m_TextContent->appendPlainText(QString("%1 added to observation list").arg(cell->objectName()));
    m_TextContent->addTextSeparator();
    m_Cells.append(cell);
    cell->setObservedOn();

    m_LabelCount->setText(QString::number(m_Cells.count()));

    m_ActionNextCell->setEnabled(true);
    m_CurrentCellIndex = m_Cells.count() - 1;
    QObject::connect(this, &QObject::destroyed, cell, &Cell::setObservedOff);
}

void DialogCellMonitor::slotClearObservationList()
{
    for(auto c: m_Cells) c->setObservedOff();
    m_Cells.clear();
    m_TextContent->clear();
    addStartText();
    m_LabelCount->setText(QString::number(m_Cells.count()));
    m_ActionNextCell->setEnabled(false);
    m_CurrentCellIndex = -1;
}

void DialogCellMonitor::slotShowNextCell()
{
    if(m_Cells.isEmpty()) { m_ActionNextCell->setEnabled(false); return; }
    if(m_CurrentCellIndex == -1 || m_CurrentCellIndex >= m_Cells.count() - 1)
    {
        m_CurrentCellIndex = 0;
        m_Scene->selectCell(m_Cells.at(m_CurrentCellIndex), false);
    }
    else
    {
        m_CurrentCellIndex++;
        m_Scene->selectCell(m_Cells.at(m_CurrentCellIndex), false);
    }
}

void DialogCellMonitor::slotAddText(const QString &text) { m_TextContent->appendHtml(QString("<p>%1</p>").arg(text)); }
void DialogCellMonitor::slotSelectedCellChanged(Cell *cell) { m_ActionSelectCell->setEnabled(cell ? true : false); }
