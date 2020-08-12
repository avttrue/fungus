#include "dialogeditrules.h"
#include "properties.h"
#include "controls.h"
#include "field/fieldrule.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QListWidget>

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
    setModal(true);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(1);
    vblForm->setSpacing(1);
    setLayout(vblForm);

    auto toolBarControl = new QToolBar();
    toolBarControl->setOrientation(Qt::Horizontal);
    toolBarControl->setMovable(false);
    toolBarControl->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto saContent = new QListWidget();

    auto toolBarMain = new QToolBar();
    toolBarMain->setOrientation(Qt::Horizontal);
    toolBarMain->setMovable(false);
    toolBarMain->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    toolBarMain->addSeparator();
    toolBarMain->addWidget(new WidgetSpacer());

    auto actionAccept = new QAction(QIcon(":/resources/img/yes.svg"), tr("Accept"));
    actionAccept->setAutoRepeat(false);
    QObject::connect(actionAccept, &QAction::triggered, [=](){ accept(); });
    toolBarMain->addAction(actionAccept);

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Cancel"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBarMain->addAction(actionCancel);

    auto actionAdd = new QAction(QIcon(":/resources/img/plus.svg"), tr("Add activity"));
    actionAdd->setAutoRepeat(false);
    //QObject::connect(actionAdd, &QAction::triggered, );

    actionDelete = new QAction(QIcon(":/resources/img/minus.svg"), tr("Delete activity"));
    actionDelete->setAutoRepeat(false);
    //QObject::connect(actionDelete, &QAction::triggered, );
    actionDelete->setEnabled(false);

    actionEdit = new QAction(QIcon(":/resources/img/edit.svg"), tr("Edit activity"));
    actionEdit->setAutoRepeat(false);
    //QObject::connect(actionEdit, &QAction::triggered, );
    actionEdit->setEnabled(false);

    actionUp = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Up activity"));
    actionUp->setAutoRepeat(false);
    //QObject::connect(actionUp, &QAction::triggered, );
    actionUp->setEnabled(false);

    actionDown = new QAction(QIcon(":/resources/img/down_arrow.svg"), tr("Down activity"));
    actionDown->setAutoRepeat(false);
    //QObject::connect(actionDown, &QAction::triggered, );
    actionDown->setEnabled(false);

    toolBarControl->addAction(actionAdd);
    toolBarControl->addAction(actionDelete);
    toolBarControl->addSeparator();
    toolBarControl->addAction(actionEdit);
    toolBarControl->addAction(actionUp);
    toolBarControl->addAction(actionDown);
    toolBarControl->addWidget(new WidgetSpacer());

    vblForm->addWidget(toolBarControl);
    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBarMain);

    installEventFilter(this);
    resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());

    qDebug() << "DialogEditRules" << windowTitle() << "created";
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogEditRules" << windowTitle() << "destroyed"; });
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
