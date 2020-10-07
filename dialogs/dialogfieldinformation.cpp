#include "dialogfieldinformation.h"
#include "infopanel.h"
#include "properties.h"
#include "controls.h"
#include "field/field.h"
#include "field/fieldinformation.h"

#include <QDebug>
#include <QToolBar>
#include <QApplication>
#include <QIcon>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMetaProperty>

DialogFieldInformation::DialogFieldInformation(QWidget *parent, const QString& title, Field* field)
    : DialogBody(parent, title, ":/resources/img/field.svg"),
      m_Field(field)
{
    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setFrameStyle(QFrame::NoFrame);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);
    glContent->setMargin(1);
    glContent->setSpacing(1);

    ToolBar()->addWidget(new WidgetSpacer());
    addDialogContent(saContent);

    loadInformation();

    QObject::connect(field, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection);
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogFieldInformation" << windowTitle() << "destroyed"; });
    qDebug() << "DialogFieldInformation" << windowTitle() << "created";
}

void DialogFieldInformation::loadInformation()
{
    glContent->addWidget(new InfoPanel(this, tr("Properties"), QVariant()));

    auto fi = m_Field->getInformation();
    auto fi_mo = fi->metaObject();

    for(int i = fi_mo->propertyOffset(); i < fi_mo->propertyCount(); ++i)
    {
        auto p = fi_mo->property(i);

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
        else if(QString(p.name()) == "MaxDensity")
            QObject::connect(fi, &FieldInformation::signalMaxDensityChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "AgeMaxDensity")
            QObject::connect(fi, &FieldInformation::signalAgeMaxDensityChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
        else if(QString(p.name()) == "CellsWithTrait")
            QObject::connect(fi, &FieldInformation::signalCellsWithTraitChanged, dip, &InfoPanel::setValue, Qt::QueuedConnection);
    }
}

Field *DialogFieldInformation::getField() const { return m_Field; }
