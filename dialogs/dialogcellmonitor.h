#ifndef DIALOGCELLMONITOR_H
#define DIALOGCELLMONITOR_H

#include <QDialog>

class QPlainTextEdit;
class Cell;

class DialogCellMonitor : public QDialog
{
    Q_OBJECT
public:
    DialogCellMonitor(QWidget *parent);

private:
    QVector<Cell*> m_Cells;
    QPlainTextEdit* m_TextContent;
};

#endif // DIALOGCELLMONITOR_H
