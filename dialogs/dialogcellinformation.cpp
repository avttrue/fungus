#include "dialogcellinformation.h"
#include "dialoginfopanel.h"
#include "properties.h"
#include "controls.h"
#include "field/cell.h"
#include "field/cellinformation.h"

#include <QDebug>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QApplication>
#include <QScrollArea>
#include <QMetaProperty>

DialogCellInformation::DialogCellInformation(QWidget *parent,
                                             Cell *cell)
    : QDialog(parent),
      m_Cell(cell)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Cell %1").arg(cell->objectName()));
    setWindowIcon(QIcon(":/resources/img/cell.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(1);
    vblForm->setSpacing(1);
    setLayout(vblForm);

    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);
    glContent->setMargin(1);
    glContent->setSpacing(1);

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto actionShowCell = new QAction(QIcon(":/resources/img/point.svg"), tr("Show cell"));
    actionShowCell->setAutoRepeat(false);
    QObject::connect(actionShowCell, &QAction::triggered, this, &DialogCellInformation::slotShowCell);
    toolBar->addAction(actionShowCell);

    toolBar->addSeparator();

    toolBar->addWidget(new WidgetSpacer());

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Close"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    loadInformation();

    QObject::connect(cell, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection); // закрывать при уничтожении cell

    installEventFilter(this);
    resize(config->CellInfoWindowWidth(), config->CellInfoWindowHeight());

    qDebug() << "DialogCellInformation" << windowTitle() << "created";
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogCellInformation" << windowTitle() << "destroyed"; });
}

bool DialogCellInformation::eventFilter(QObject *object, QEvent *event)
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
    case QEvent::Close:
    {
        if(object != this || isMinimized() || isMaximized()) return false;

        config->setCellInfoWindowWidth(width());
        config->setCellInfoWindowHeight(height());

        return true;
    }
    default: { return false; }
    }
}

void DialogCellInformation::loadInformation()
{
    glContent->addWidget(new DialogInfoPanel(this, tr("Properties"), QVariant()));

    auto ci = m_Cell->getNewInfo();
    auto ci_mo = ci->metaObject();

    for(int i = ci_mo->propertyOffset(); i < ci_mo->propertyCount(); ++i)
    {
        auto p = ci_mo->property(i);
        auto value = ci->property(p.name());
        auto dip = new DialogInfoPanel(this, p.name(), value);

        glContent->addWidget(dip);

        if(QString(p.name()) == "Age")
            QObject::connect(ci, &CellInformation::signalAgeChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "State")
            QObject::connect(ci, &CellInformation::signalStateChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "Generation")
            QObject::connect(ci, &CellInformation::signalGenerationChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
    }
}

bool DialogCellInformation::FindPreviousCopy(Cell *cell)
{    
    for (QWidget *widget: QApplication::topLevelWidgets())
    {
        auto dci = qobject_cast<DialogCellInformation*>(widget);

        if (!dci) continue;

        if(dci->getCell() == cell)
        {
            dci->showNormal();
            dci->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}

void DialogCellInformation::slotShowCell() { Q_EMIT signalShowCell(m_Cell); }
Cell *DialogCellInformation::getCell() const { return m_Cell; }
