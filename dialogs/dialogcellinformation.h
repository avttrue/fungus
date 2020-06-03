#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

const QSize WINDOW_SIZE(300, 200);

class QTextBrowser;
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
    void loadInformation();

private:
    QTextBrowser* textBrowser;
    Cell* m_Cell;
};


#endif // DIALOGCELLINFORMATION_H
