#ifndef DIALOGEDITRULES_H
#define DIALOGEDITRULES_H

#include <QDialog>

class FieldRule;
class QGridLayout;
class QListWidget;
class ClickableLabel;
class QListWidgetItem;

class DialogEditRules : public QDialog
{
    Q_OBJECT
public:
    DialogEditRules(QWidget *parent, FieldRule* rules);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void addContentItem(const QString& text);
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
    void slotActionAdd();
    void slotActionDelete();
    void slotActionUp();
    void slotActionDown();
    void slotActionEdit();
    void slotEditRules();
    void slotItemDoubleClicked(QListWidgetItem *item);
};

#endif // DIALOGEDITRULES_H
