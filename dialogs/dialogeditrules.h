#ifndef DIALOGEDITRULES_H
#define DIALOGEDITRULES_H

#include "dialogbody.h"

#include "field/fieldrule.h"

class FieldRule;
class QGridLayout;
class QListWidget;
class ClickableLabel;
class QListWidgetItem;

class DialogEditRules : public DialogBody
{
    Q_OBJECT
public:
    DialogEditRules(QWidget *parent, FieldRule* rules);

protected:
    void addContentItem(const QString& text, Kernel::ActivityType type);
    void loadContent();
    void slotRowChanged(int value);
    bool editActivityElement(QVector<QVariant>* element);
    void editActivity(int index);

private:
    FieldRule* m_Rule;
    QAction* m_ActionDelete;
    QAction* m_ActionUp;
    QAction* m_ActionDown;
    QAction* m_ActionEdit;
    QListWidget* m_lwContent;
    ClickableLabel* m_RulesProperties;

private Q_SLOTS:
    void slotActionAccept();
    void slotActionAdd();
    void slotActionDelete();
    void slotActionUp();
    void slotActionDown();
    void slotActionEdit();
    void slotEditRules();
    void slotItemDoubleClicked(QListWidgetItem *item);
};

#endif // DIALOGEDITRULES_H
