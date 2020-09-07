#include "dialogfieldinformation.h"
#include "infopanel.h"
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

DialogFieldInformation::DialogFieldInformation(QWidget *parent, const QString& title, Field* field)
    : QDialog(parent),
      m_Field(field)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(title);
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

    toolBar->addWidget(new WidgetSpacer());

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Close"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    loadInformation();

    installEventFilter(this);
    resize(config->FieldInfoWindowWidth(), config->FieldInfoWindowHeight());

    QObject::connect(field, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection);
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogFieldInformation" << windowTitle() << "destroyed"; });
    qDebug() << "DialogFieldInformation" << windowTitle() << "created";
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
    case QEvent::Hide:
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
    glContent->addWidget(new InfoPanel(this, tr("Properties"), QVariant()));

    auto fi = m_Field->getInformation();
    auto fi_mo = fi->metaObject();

    for(int i = fi_mo->propertyOffset(); i < fi_mo->propertyCount(); ++i)
    {
        auto p = fi_mo->property(i);

        if(QString(p.name()) == "AverageCalc") continue;

        auto value = fi->property(p.name());
        auto dip = new InfoPanel(this, p.name(), value);

        glContent->addWidget(dip);

        if(QString(p.name()) == "Age")
            QObject::connect(fi, &FieldInformation::signalAgeChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "DeadCells")
            QObject::connect(fi, &FieldInformation::signalDeadCellsChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "AliveCells")
            QObject::connect(fi, &FieldInformation::signalAliveCellsChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "CursedCells")
            QObject::connect(fi, &FieldInformation::signalCursedCellsChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "ActiveCells")
            QObject::connect(fi, &FieldInformation::signalActiveCellsChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "LastActiveAge")
            QObject::connect(fi, &FieldInformation::signalLastActiveAgeChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "Density")
            QObject::connect(fi, &FieldInformation::signalDensityChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "ChangedCells")
            QObject::connect(fi, &FieldInformation::signalChangedCellsChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
    }
}

Field *DialogFieldInformation::getField() const { return m_Field; }
