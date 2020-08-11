#include "dialogfieldinformation.h"
#include "dialoginfopanel.h"
#include "properties.h"
#include "controls.h"
#include "field/field.h"
#include "field/fieldinformation.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QScrollArea>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QMetaProperty>

DialogFieldInformation::DialogFieldInformation(QWidget *parent, Field* field)
    : QDialog(parent),
      m_Field(field)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Field"));
    setWindowIcon(QIcon(":/resources/img/field.svg"));
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

    toolBar->addWidget(new WidgetSpacer(this));

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), "Cancel");
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    loadInformation();

    QObject::connect(field, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection); // закрывать при уничтожении field

    installEventFilter(this);
    resize(config->FieldInfoWindowWidth(), config->FieldInfoWindowHeight());
}

bool DialogFieldInformation::FindPreviousCopy(Field *field)
{
    for (QWidget *widget: QApplication::topLevelWidgets())
    {
        auto dfi = qobject_cast<DialogFieldInformation*>(widget);

        if (!dfi) continue;

        if(dfi->getField() == field)
        {
            dfi->showNormal();
            dfi->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}

bool DialogFieldInformation::eventFilter(QObject *object, QEvent *event)
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

        config->setFieldInfoWindowWidth(width());
        config->setFieldInfoWindowHeight(height());
        return true;
    }
    default: { return false; }
    }
}

void DialogFieldInformation::loadInformation()
{
    glContent->addWidget(new DialogInfoPanel(this, tr("Properties"), QVariant()));

    auto fi = m_Field->getInformation();
    auto fi_mo = fi->metaObject();

    for(int i = fi_mo->propertyOffset(); i < fi_mo->propertyCount(); ++i)
    {
        auto p = fi_mo->property(i);

        if(QString(p.name()) == "AverageCalc") continue;

        auto value = fi->property(p.name());
        auto dip = new DialogInfoPanel(this, p.name(), value);

        glContent->addWidget(dip);

        if(QString(p.name()) == "Age")
            QObject::connect(fi, &FieldInformation::signalAgeChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "DeadCells")
            QObject::connect(fi, &FieldInformation::signalDeadCellsChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "AliveCells")
            QObject::connect(fi, &FieldInformation::signalAliveCellsChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "CursedCells")
            QObject::connect(fi, &FieldInformation::signalCursedCellsChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "ActiveCells")
            QObject::connect(fi, &FieldInformation::signalActiveCellsChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "LastActiveAge")
            QObject::connect(fi, &FieldInformation::signalLastActiveAgeChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "Density")
            QObject::connect(fi, &FieldInformation::signalDensityChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "ChangedCells")
            QObject::connect(fi, &FieldInformation::signalChangedCellsChanged, dip, &DialogInfoPanel::setValue, Qt::QueuedConnection);
    }
}

Field *DialogFieldInformation::getField() const { return m_Field; }
