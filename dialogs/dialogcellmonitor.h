#ifndef DIALOGCELLMONITOR_H
#define DIALOGCELLMONITOR_H

#include "dialogbody.h"

class Cell;
class Scene;
class TextLog;
class QLabel;

class DialogCellMonitor : public DialogBody
{
    Q_OBJECT
public:
    DialogCellMonitor(QWidget *parent, const QString &title, Scene* scene);

private:
    Scene* m_Scene;
    QVector<Cell*> m_Cells;
    QAction* m_ActionSelectCell;
    QAction* m_ActionNextCell;
    TextLog* m_TextContent;
    QLabel* m_LabelCount;
    int m_CurrentCellIndex;

protected:
    void addStartText();

private Q_SLOTS:
    void slotAddText(const QString &text);
    void slotSelectedCellChanged(Cell* cell);
    void slotAddCell();
    void slotClearObservationList();
    void slotShowNextCell();
};

#endif // DIALOGCELLMONITOR_H
