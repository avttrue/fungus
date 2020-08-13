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

    m_RulesProperties = new ClickableLabel("");
    m_RulesProperties->setFont(QFont("monospace", -1, QFont::Bold));
    QObject::connect(m_RulesProperties, &ClickableLabel::signalClicked, this, &DialogEditRules::slotEditRules);

    m_lwContent = new QListWidget();
    m_lwContent->setIconSize({config->ButtonSize(), config->ButtonSize()});
    QObject::connect(m_lwContent, &QListWidget::currentRowChanged, this, &DialogEditRules::slotRowChanged);

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
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionAdd);

    m_ActionDelete = new QAction(QIcon(":/resources/img/minus.svg"), tr("Delete activity"));
    m_ActionDelete->setAutoRepeat(false);
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionDelete);
    m_ActionDelete->setEnabled(false);

    m_ActionEdit = new QAction(QIcon(":/resources/img/edit.svg"), tr("Edit activity"));
    m_ActionEdit->setAutoRepeat(false);
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionEdit);
    m_ActionEdit->setEnabled(false);

    m_ActionUp = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Up activity"));
    m_ActionUp->setAutoRepeat(false);
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionUp);
    m_ActionUp->setEnabled(false);

    m_ActionDown = new QAction(QIcon(":/resources/img/down_arrow.svg"), tr("Down activity"));
    m_ActionDown->setAutoRepeat(false);
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionDown);
    m_ActionDown->setEnabled(false);

    toolBarControl->addAction(actionAdd);
    toolBarControl->addAction(m_ActionDelete);
    toolBarControl->addSeparator();
    toolBarControl->addAction(m_ActionEdit);
    toolBarControl->addAction(m_ActionUp);
    toolBarControl->addAction(m_ActionDown);
    toolBarControl->addWidget(new WidgetSpacer());

    vblForm->addWidget(m_RulesProperties);
    vblForm->addWidget(toolBarControl);
    vblForm->addWidget(m_lwContent);
    vblForm->addWidget(toolBarMain);

    loadContent();

    resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());

    installEventFilter(this);

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
    auto o = qobject_cast<DialogEditRules*>(object);
    if(o)
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

            config->setEditRulesWindowWidth(width());
            config->setEditRulesWindowHeight(height());
            return true;
        }
        default: { return false; }
        }
    }
    else
    {
        switch (event->type())
        {
        case QEvent::Wheel:
        { return true; }
        default: { return false; }
        }
    }
}

void DialogEditRules::loadContent()
{
    if(!m_Rules)
    {
       qCritical() << "Rules is null";
       return;
    }

    m_ActionUp->setDisabled(true);
    m_ActionDown->setDisabled(true);
    m_ActionDelete->setDisabled(true);
    m_ActionEdit->setDisabled(true);

    m_RulesProperties->setText(m_Rules->PropertiesToString());

    m_lwContent->clear();
    for(auto a: m_Rules->getActivity())
    {
       auto item = new QListWidgetItem(QIcon(":/resources/img/running.svg"), ActivityElementToString(a), m_lwContent);
       item->setData(Qt::FontRole, QFont("monospace", -1, QFont::Bold));
       m_lwContent->addItem(item);
    }
}

void DialogEditRules::slotRowChanged(int value)
{
    if(value < 0)
    {
        m_ActionUp->setDisabled(true);
        m_ActionDown->setDisabled(true);
        m_ActionDelete->setDisabled(true);
        m_ActionEdit->setDisabled(true);
        return;
    }
    if(value == 0)
    {
        m_ActionUp->setDisabled(true);
        m_ActionDown->setDisabled(m_lwContent->count() == 1);
        m_ActionDelete->setDisabled(false);
        m_ActionEdit->setDisabled(false);
        return;
    }
    if(value == m_lwContent->count() - 1)
    {
        m_ActionUp->setDisabled(false);
        m_ActionDown->setDisabled(true);
        m_ActionDelete->setDisabled(false);
        m_ActionEdit->setDisabled(false);
        return;
    }
    if(value > 0 && value < m_lwContent->count() - 1)
    {
        m_ActionUp->setDisabled(false);
        m_ActionDown->setDisabled(false);
        m_ActionDelete->setDisabled(false);
        m_ActionEdit->setDisabled(false);
        return;
    }
}

void DialogEditRules::slotActionAdd()
{
    // TODO: slotActionAdd
}

void DialogEditRules::slotActionDelete()
{
    // TODO: slotActionDelete
}

void DialogEditRules::slotActionUp()
{
    // TODO: slotActionUp
}

void DialogEditRules::slotActionDown()
{
    // TODO: slotActionDown
}

void DialogEditRules::slotActionEdit()
{
    // TODO: slotActionEdit
}

void DialogEditRules::slotEditRules()
{
    // TODO: slotEditRules
}
