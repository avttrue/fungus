#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

const QSize WINDOW_SIZE(500, 400);

class QGridLayout;
class Cell;

class DialogCellInformation : public QDialog
{
    Q_OBJECT
public:
    DialogCellInformation(QWidget *parent,
                          Cell* cell);

    Cell *getCell() const;
    static bool FindPreviousCopy(Cell* cell); // найти предыдущую копию окна, если найдена, то возвращается TRUE

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QGridLayout* glContent;
    Cell* m_Cell;
};


#endif // DIALOGCELLINFORMATION_H
