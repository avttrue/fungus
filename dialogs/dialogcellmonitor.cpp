#include "dialogcellmonitor.h"
#include "properties.h"
#include "controls.h"

#include <QDebug>
#include <QIcon>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QVBoxLayout>

DialogCellMonitor::DialogCellMonitor(QWidget *parent)
    :QDialog(parent)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Cells minitor"));
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
    actionClose->setAutoRepeat(false);
    QObject::connect(actionClose, &QAction::triggered, [=](){ close(); });


    toolBarMain->addAction(actionClose);
    toolBarMain->addWidget(new WidgetSpacer());

    m_TextContent = new QPlainTextEdit(this);
    m_TextContent->setUndoRedoEnabled(false);
    m_TextContent->setReadOnly(true);

    vblForm->addWidget(m_TextContent);
    vblForm->addWidget(toolBarMain);



    resize(config->CellMonitorWindowWidth(), config->CellMonitorWindowHeight());

    qDebug() << "DialogCellMonitor" << windowTitle() << "created";
    QObject::connect(this, &QObject::destroyed,
                     [=]() { qDebug() << "DialogCellMonitor" << windowTitle() << "destroyed"; });
}
