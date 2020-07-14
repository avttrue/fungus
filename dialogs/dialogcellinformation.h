#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

class Cell;
class QGridLayout;

class DialogCellInformation : public QDialog
{
    Q_OBJECT

public:
    DialogCellInformation(QWidget *parent, Cell* cell);
    Cell *getCell() const;
    void setValue(const QString& key, const QVariant &value);
    static bool FindPreviousCopy(Cell* cell); // найти предыдущую копию окна, если найдена, то возвращается TRUE и делает его активным

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void loadInformation();

private:
    Cell* m_Cell;
    QGridLayout* glContent;

private Q_SLOTS:
    void slotShowPoint();
    void slotCellAgeChanged(qint64 value);
    void slotCellStateChanged(int value);
    void slotCellGenerationChanged(qint64 value);
    void slotCellActivityChanged(bool value);

Q_SIGNALS:
    void signalShowCell(Cell* cell);

};

#endif // DIALOGCELLINFORMATION_H
