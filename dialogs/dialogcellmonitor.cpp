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

DialogCellMonitor::DialogCellMonitor(QWidget *parent, const QString& title, Scene *scene)
    :DialogBody(parent, title, ":/resources/img/cell_monitor.svg"),
      m_Scene(scene),
      m_CurrentCellIndex(-1)
{
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

    ToolBar()->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    ToolBar()->addAction(m_ActionSelectCell);
    ToolBar()->addAction(m_ActionNextCell);
    ToolBar()->addSeparator();
    ToolBar()->addAction(actionClear);
    ToolBar()->addSeparator();
    ToolBar()->addWidget(new QLabel("Observation list: ", this));
    ToolBar()->addWidget(m_LabelCount);
    ToolBar()->addWidget(new WidgetSpacer());

    m_TextContent = new TextLog(this);

    addDialogContent(m_TextContent);

    slotSelectedCellChanged(m_Scene->getSelectedCell());
    QObject::connect(m_Scene, &Scene::signalSelectedCellChanged, this, &DialogCellMonitor::slotSelectedCellChanged);
    QObject::connect(m_Scene->getField(), &Field::signalRuleMessage, this, &DialogCellMonitor::slotAddText);
    QObject::connect(m_Scene, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection);
    QObject::connect(this, &QObject::destroyed,
                     [=]() { qDebug() << "DialogCellMonitor" << windowTitle() << "destroyed"; });

    if(scene->getSelectedCell()) slotAddCell();
    else slotAddText(tr("Select a cell and push selection button"));

    qDebug() << "DialogCellMonitor" << windowTitle() << "created";
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

    m_TextContent->addTextSeparator();
    m_TextContent->appendPlainText(QString("%1 added to observation list").arg(cell->objectName()));
    m_TextContent->addTextSeparator();
    m_Cells.append(cell);
    cell->setObservedOn();

    m_LabelCount->setText(QString::number(m_Cells.count()));

    m_ActionNextCell->setEnabled(true);
    m_CurrentCellIndex = m_Cells.count() - 1;

    qDebug() << "Observation list count:" << m_Cells.count();
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
