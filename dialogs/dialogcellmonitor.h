#ifndef DIALOGCELLMONITOR_H
#define DIALOGCELLMONITOR_H

#include <QDialog>

class Cell;
class Scene;
class TextLog;
class QLabel;

class DialogCellMonitor : public QDialog
{
    Q_OBJECT
public:
    DialogCellMonitor(QWidget *parent, Scene* scene, const QString &title);

private:
    Scene* m_Scene;
    QVector<Cell*> m_Cells;
    QAction* m_ActionSelectCell;
    QAction* m_ActionNextCell;
    TextLog* m_TextContent;
    QLabel* m_LabelCount;
    int m_CurrentCellIndex;

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void addStartText();

private Q_SLOTS:
    void slotAddText(const QString &text);
    void slotSelectedCellChanged(Cell* cell);
    void slotAddCell();
    void slotClearObservationList();
    void slotShowNextCell();
};

#endif // DIALOGCELLMONITOR_H
