#ifndef DIALOGEDITRULES_H
#define DIALOGEDITRULES_H

#include <QDialog>

class FieldRule;
class QGridLayout;
class QListWidget;

class DialogEditRules : public QDialog
{
    Q_OBJECT
public:
    DialogEditRules(QWidget *parent, FieldRule* rules);
    static bool FindPreviousCopy(); // найти предыдущую копию окна, если найдена, то возвращается TRUE и делает его активным

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void loadContent();

private:
    FieldRule* m_Rules;
    QAction* actionDelete;
    QAction* actionUp;
    QAction* actionDown;
    QAction* actionEdit;
    QListWidget* m_lwContent;
};

#endif // DIALOGEDITRULES_H
