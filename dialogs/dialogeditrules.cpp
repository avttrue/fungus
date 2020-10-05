#include "dialogeditrules.h"
#include "properties.h"
#include "controls.h"
#include "dialogvalueslist.h"

#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QListWidget>

DialogEditRules::DialogEditRules(QWidget *parent, FieldRule* rules)
    : DialogBody(parent, tr("Edit rule"), ":/resources/img/rule.svg", true, true),
      m_Rule(rules)
{
    auto toolBarControl = new QToolBar();
    toolBarControl->setOrientation(Qt::Horizontal);
    toolBarControl->setMovable(false);
    toolBarControl->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    m_RulesProperties = new ClickableLabel("");
    m_RulesProperties->setFont(QFont("monospace", -1, QFont::Bold));
    m_RulesProperties->setToolTip(tr("Click to edit"));
    QObject::connect(m_RulesProperties, &ClickableLabel::signalClicked, this, &DialogEditRules::slotEditRules);

    m_lwContent = new QListWidget();
    m_lwContent->setIconSize({config->ButtonSize(), config->ButtonSize()});
    QObject::connect(m_lwContent, &QListWidget::currentRowChanged, this, &DialogEditRules::slotRowChanged);
    QObject::connect(m_lwContent, &QListWidget::itemDoubleClicked, this, &DialogEditRules::slotItemDoubleClicked);

    auto actionAccept = new QAction(QIcon(":/resources/img/yes.svg"), tr("Accept"));
    actionAccept->setAutoRepeat(false);
    QObject::connect(actionAccept, &QAction::triggered, this, &DialogEditRules::slotActionAccept);

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Cancel"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });

    auto actionUpdate = new QAction(QIcon(":/resources/img/update.svg"), tr("Update"));
    actionUpdate->setAutoRepeat(false);
    QObject::connect(actionUpdate, &QAction::triggered, this, &DialogEditRules::loadContent);

    auto actionAdd = new QAction(QIcon(":/resources/img/plus.svg"), tr("Add activity element"));
    actionAdd->setAutoRepeat(false);
    QObject::connect(actionAdd, &QAction::triggered, this, &DialogEditRules::slotActionAdd);

    m_ActionDelete = new QAction(QIcon(":/resources/img/minus.svg"), tr("Delete activity element"));
    m_ActionDelete->setAutoRepeat(false);
    QObject::connect(m_ActionDelete, &QAction::triggered, this, &DialogEditRules::slotActionDelete);
    m_ActionDelete->setEnabled(false);

    m_ActionEdit = new QAction(QIcon(":/resources/img/edit.svg"), tr("Edit activity element"));
    m_ActionEdit->setAutoRepeat(false);
    QObject::connect(m_ActionEdit, &QAction::triggered, this, &DialogEditRules::slotActionEdit);
    m_ActionEdit->setEnabled(false);

    m_ActionUp = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Up activity element"));
    m_ActionUp->setAutoRepeat(false);
    QObject::connect(m_ActionUp, &QAction::triggered, this, &DialogEditRules::slotActionUp);
    m_ActionUp->setEnabled(false);

    m_ActionDown = new QAction(QIcon(":/resources/img/down_arrow.svg"), tr("Down activity element"));
    m_ActionDown->setAutoRepeat(false);
    QObject::connect(m_ActionDown, &QAction::triggered, this, &DialogEditRules::slotActionDown);
    m_ActionDown->setEnabled(false);

    toolBarControl->addAction(actionAdd);
    toolBarControl->addAction(m_ActionDelete);
    toolBarControl->addSeparator();
    toolBarControl->addAction(m_ActionEdit);
    toolBarControl->addAction(m_ActionUp);
    toolBarControl->addAction(m_ActionDown);
    toolBarControl->addWidget(new WidgetSpacer());

    ToolBar()->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    ToolBar()->addAction(actionUpdate);
    ToolBar()->addWidget(new WidgetSpacer());
    ToolBar()->addAction(actionAccept);

    addDialogContent(m_RulesProperties);
    addDialogContent(toolBarControl);
    addDialogContent(m_lwContent);

    loadContent();

    resize(config->EditRulesWindowWidth(), config->EditRulesWindowHeight());

    installEventFilter(this);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogEditRules" << windowTitle() << "destroyed"; });
    qDebug() << "DialogEditRules" << windowTitle() << "created";
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

void DialogEditRules::addContentItem(const QString &text, Kernel::ActivityType type)
{
    auto item = new QListWidgetItem(ActivityTypeToIcon(type), text, m_lwContent);
    item->setData(Qt::FontRole, QFont("monospace", -1, QFont::Bold));
    m_lwContent->addItem(item);
}

void DialogEditRules::loadContent()
{
    qDebug() << __func__;
    if(!m_Rule)
    {
        qCritical() << "Rule is null";
        return;
    }

    auto row = m_lwContent->currentRow();
    m_ActionUp->setDisabled(true);
    m_ActionDown->setDisabled(true);
    m_ActionDelete->setDisabled(true);
    m_ActionEdit->setDisabled(true);

    m_RulesProperties->setText(m_Rule->PropertiesToString());

    m_lwContent->clear();
    for(auto a: m_Rule->getActivity())
    {
        auto type = static_cast<Kernel::ActivityType>(a.at(0).toInt());
        addContentItem(ActivityElementToString(a), type);
    }

    auto count = m_lwContent->count();
    if(row > -1 && row < count) m_lwContent->setCurrentRow(row);
    else if(count) m_lwContent->setCurrentRow(0);
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

bool DialogEditRules::editActivityElement(QVector<QVariant> *element)
{
    /* {ActivityType,
     * SelfState,
     * ActivityTarget,
     * TargetState,
     * ActivityOperand,
     * ActivityOperator,
     * [значение],
     * Break} */
    qDebug() << __func__;
    auto atypelist = listKernelEnum("ActivityType");
    auto statelist = listKernelEnum("CellState");
    auto atstatelist = listKernelEnum("CellState");
    atstatelist.removeAll("ANY");
    atstatelist.removeAll("NOT_CURSED");
    atstatelist.removeAll("NOT_ALIVE");
    auto atargetlist = listKernelEnum("ActivityTarget");
    auto aoperand = listKernelEnum("ActivityOperand");
    auto aoperator = listKernelEnum("ActivityOperator");

    const QVector<QString> keys = {
        tr("00#_Set"), "01#__",
        tr("02#_If cell is"), "03#__",
        tr("04#_And"), "05#__",
        tr("06#_Is"), "07#__", "08#__", "09#__", tr("10#__value"),
        tr("11#_and then Abort"), tr("12#__yes / no"),
    };
    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::StringList, QVariant::fromValue(element->at(0)), 0, atypelist, DialogValueMode::OneFromList}},
     {keys.at(2), {}},
     {keys.at(3), {QVariant::StringList, QVariant::fromValue(element->at(1)), 0, statelist, DialogValueMode::OneFromList}},
     {keys.at(4), {}},
     {keys.at(5), {QVariant::StringList, QVariant::fromValue(element->at(2)), 0, atargetlist, DialogValueMode::OneFromList}},
     {keys.at(6), {}},
     {keys.at(7), {QVariant::StringList, QVariant::fromValue(element->at(3)), 0, atstatelist, DialogValueMode::OneFromList}},
     {keys.at(8), {QVariant::StringList, QVariant::fromValue(element->at(4)), 0, aoperand, DialogValueMode::OneFromList}},
     {keys.at(9), {QVariant::StringList, QVariant::fromValue(element->at(5)), 0, aoperator, DialogValueMode::OneFromList}},
     {keys.at(10), {QVariant::Int, element->at(6).toInt(), 0, 0}},
     {keys.at(11), {}},
     {keys.at(12), {QVariant::Bool, element->at(7).toBool()}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/edit.svg", tr("Activity element"), &map);
    dvl->resize(config->EditActivityWindowWidth(), config->EditActivityWindowHeight());
    QObject::connect(dvl, &DialogValuesList::signalSizeChanged, [=](QSize size)
    {
        config->setEditActivityWindowWidth(size.width());
        config->setEditActivityWindowHeight(size.height());
    });
    if(!dvl->exec()) return false;

    element->data()[0].setValue(static_cast<Kernel::ActivityType>(atypelist.indexOf(map.value(keys.at(1)).value.toString())));
    element->data()[1].setValue(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(3)).value.toString())));
    element->data()[2].setValue(static_cast<Kernel::ActivityTarget>(atargetlist.indexOf(map.value(keys.at(5)).value.toString())));
    element->data()[3].setValue(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(7)).value.toString())));
    element->data()[4].setValue(static_cast<Kernel::ActivityOperand>(aoperand.indexOf(map.value(keys.at(8)).value.toString())));
    element->data()[5].setValue(static_cast<Kernel::ActivityOperator>(aoperator.indexOf(map.value(keys.at(9)).value.toString())));
    element->data()[6].setValue(map.value(keys.at(10)).value.toInt());
    element->data()[7].setValue(map.value(keys.at(12)).value.toBool());
    return true;
}

void DialogEditRules::editActivity(int index)
{
    qDebug() << __func__;
    auto element = m_Rule->getActivity().at(index);
    if(element.count() != RULE_ACTIVITY_LEN)
    {
        qCritical() << "Rule activity element #" << index << "length is incorrect:" << element.count();
        return;
    }

    if(!editActivityElement(&element)) return;

    auto act = m_Rule->getActivity();
    act[index] = element;
    m_Rule->setActivity(act);

    m_lwContent->item(index)->setText(ActivityElementToString(element));
    auto type = static_cast<Kernel::ActivityType>(element.at(0).toInt());
    m_lwContent->item(index)->setIcon(ActivityTypeToIcon(type));

}

void DialogEditRules::slotActionAccept()
{
    if(m_Rule->objectName().isEmpty())
        QMessageBox::warning(this, tr("Warning"), tr("Rule name is empty."));
    else
        accept();
}

void DialogEditRules::slotActionAdd()
{
    QVector<QVariant> element = { QVariant::fromValue(Kernel::ActivityType::BIRTH),
                                  QVariant::fromValue(Kernel::CellState::DEAD),
                                  QVariant::fromValue(Kernel::ActivityTarget::NEAR),
                                  QVariant::fromValue(Kernel::CellState::ALIVE),
                                  QVariant::fromValue(Kernel::ActivityOperand::COUNT),
                                  QVariant::fromValue(Kernel::ActivityOperator::EQUAL),
                                  0, true};

    if(!editActivityElement(&element)) return;

    auto act = m_Rule->getActivity();
    act.append(element);
    m_Rule->setActivity(act);

    auto type = static_cast<Kernel::ActivityType>(element.at(0).toInt());
    addContentItem(ActivityElementToString(element), type);
    m_lwContent->setCurrentRow(m_lwContent->count() -1);
}

void DialogEditRules::slotActionDelete()
{
    auto row = m_lwContent->currentRow();
    auto act = m_Rule->getActivity();

    act.remove(row);
    m_Rule->setActivity(act);

    m_lwContent->takeItem(row);
    m_lwContent->setCurrentRow(-1);
}

void DialogEditRules::slotActionUp()
{
    auto row = m_lwContent->currentRow();
    if(row <= 0) return;

    auto act = m_Rule->getActivity();
    act.swapItemsAt(row, row - 1);
    m_Rule->setActivity(act);

    auto lwi = m_lwContent->takeItem(row);
    m_lwContent->insertItem(row - 1, lwi);
    m_lwContent->setCurrentRow(row - 1);
}

void DialogEditRules::slotActionDown()
{
    auto row = m_lwContent->currentRow();
    if(row >= m_lwContent->count()) return;

    auto act = m_Rule->getActivity();
    act.swapItemsAt(row, row + 1);
    m_Rule->setActivity(act);

    auto lwi = m_lwContent->takeItem(row);
    m_lwContent->insertItem(row + 1, lwi);
    m_lwContent->setCurrentRow(row + 1);
}

void DialogEditRules::slotActionEdit()
{
    editActivity(m_lwContent->currentRow());
}

void DialogEditRules::slotEditRules()
{
    const QVector<QString> keys = {
        tr("00#_Rule name"), "01#_",
        tr("02#_Rule description"), tr("03#_"),
        tr("04#_The time during which the cell will remain cursed (always: -1)"), tr("05#_value"),
    };

    QMap<QString, DialogValue> map =
    {{keys.at(0), {}},
     {keys.at(1), {QVariant::String,  m_Rule->objectName(), "", ""}},
     {keys.at(2), {}},
     {keys.at(3), {QVariant::String,  m_Rule->getDescription(), "", ""}},
     {keys.at(4), {}},
     {keys.at(5), {QVariant::Int,  m_Rule->getCurseTime(), -1, -1}},
    };

    auto dvl = new DialogValuesList(this, ":/resources/img/edit.svg", tr("Rule property"), &map);
    dvl->resize(config->RulePropertyWindowWidth(), config->RulePropertyWindowHeight());
    QObject::connect(dvl, &DialogValuesList::signalSizeChanged, [=](QSize size)
    {
        config->setRulePropertyWindowWidth(size.width());
        config->setRulePropertyWindowHeight(size.height());
    });
    if(!dvl->exec()) return;

    m_Rule->setObjectName(map.value(keys.at(1)).value.toString().simplified());
    m_Rule->setDescription(map.value(keys.at(3)).value.toString());
    m_Rule->setCurseTime(map.value(keys.at(5)).value.toInt());

    m_RulesProperties->setText(m_Rule->PropertiesToString());
}

void DialogEditRules::slotItemDoubleClicked(QListWidgetItem *item)
{
    if(!item) return;
    auto row = m_lwContent->row(item);
    editActivity(row);
}
