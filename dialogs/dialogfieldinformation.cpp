#include "dialogfieldinformation.h"
#include "dialoginfopanel.h"
#include "properties.h"
#include "controls.h"
#include "field/field.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QScrollArea>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>


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

    //loadInformation();


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
        if(!isMinimized()) return false;

        // если всё же свернули
        setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
        return true;

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

Field *DialogFieldInformation::getField() const { return m_Field; }
