#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

const QSize WINDOW_SIZE(200, 300);
const auto TB_PROPERTY_CONTENT = "RealHTML";
const auto TAG_YES = "<div style='text-align: center; color: white; background-color: green;'>&#160;V&#160;</div>";
const auto TAG_NO =  "<div style='text-align: center; color: white; background-color: red;'>&#160;X&#160;</div>";

class QTextBrowser;
class Cell;

class DialogCellInformation : public QDialog
{
    Q_OBJECT
public:
    DialogCellInformation(QWidget *parent,
                          Cell* cell);

    Cell *getCell() const;
    static bool FindPreviousCopy(Cell* cell); // найти предыдущую копию окна, если найдена, то возвращается TRUE и делает его активным

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void loadInformation();
    void saveContent();

private:
    QTextBrowser* textBrowser;
    Cell* m_Cell;
};


#endif // DIALOGCELLINFORMATION_H
