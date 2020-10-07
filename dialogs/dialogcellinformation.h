#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include "dialogbody.h"

class Cell;
class QGridLayout;

class DialogCellInformation : public DialogBody
{
    Q_OBJECT
public:
    DialogCellInformation(QWidget *parent, Cell* cell);
    Cell *getCell() const;
    static bool FindPreviousCopy(Cell* cell); // найти предыдущую копию окна, если найдена, то возвращается TRUE и делает его активным

protected:
    void loadInformation();

private:
    Cell* m_Cell;
    QGridLayout* glContent;

private Q_SLOTS:
    void slotShowCell();

Q_SIGNALS:
    void signalShowCell(Cell* cell);
};

#endif // DIALOGCELLINFORMATION_H
