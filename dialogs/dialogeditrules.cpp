#include "dialogeditrules.h"
#include "properties.h"
#include "controls.h"
#include "field/fieldrule.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QScrollArea>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>

DialogEditRules::DialogEditRules(QWidget *parent, FieldRule* rules)
    : QDialog(parent),
      m_Rules(rules)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Edit rules"));
    setWindowIcon(QIcon(":/resources/img/rule.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(1);
    vblForm->setSpacing(1);
    setLayout(vblForm);

    auto toolBarControl = new QToolBar();
    toolBarControl->setOrientation(Qt::Horizontal);
    toolBarControl->setMovable(false);
    toolBarControl->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

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

    auto toolBarMain = new QToolBar();
    toolBarMain->setOrientation(Qt::Horizontal);
    toolBarMain->setMovable(false);
    toolBarMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    toolBarMain->addSeparator();
    toolBarMain->addWidget(new WidgetSpacer(this));

    auto actionAccept = new QAction(QIcon(":/resources/img/yes.svg"), tr("Accept"));
    actionAccept->setAutoRepeat(false);
    QObject::connect(actionAccept, &QAction::triggered, [=](){ accept(); });
    toolBarMain->addAction(actionAccept);

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Cancel"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBarMain->addAction(actionCancel);

    auto actionAdd = new QAction(QIcon(":/resources/img/plus.svg"), tr("Add rule"));
    actionAdd->setAutoRepeat(false);
    //QObject::connect(actionAdd, &QAction::triggered, );
    toolBarControl->addAction(actionAdd);

    auto actionDelete = new QAction(QIcon(":/resources/img/minus.svg"), tr("Delete rule"));
    actionDelete->setAutoRepeat(false);
    //QObject::connect(actionDelete, &QAction::triggered, );
    toolBarControl->addAction(actionDelete);

    toolBarControl->addWidget(new WidgetSpacer(this));

    vblForm->addWidget(toolBarControl);
    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBarMain);


    installEventFilter(this);
    resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());
}

bool DialogEditRules::FindPreviousCopy()
{
    for (QWidget *widget: QApplication::topLevelWidgets())
    {
        auto der = qobject_cast<DialogEditRules*>(widget);

        if (!der) continue;

        if(QString(der->metaObject()->className()) == "DialogEditRules")
        {
            der->showNormal();
            der->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}

bool DialogEditRules::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Wheel:
    { return true; }
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

        config->setEditRulesWindowWidth(width());
        config->setEditRulesWindowHeight(height());
        return true;
    }
    default: { return false; }
    }
}
