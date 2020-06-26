#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

const auto TB_PROPERTY_CONTENT = "RealHTML";

class QTextEdit;
class Cell;
class QCheckBox;

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

private Q_SLOTS:
    void slotSaveContent();
    void slotShowPoint();

private:
    QTextEdit* m_TEContent;
    Cell* m_Cell;
    QCheckBox* m_CBShowRule;
};


#endif // DIALOGCELLINFORMATION_H