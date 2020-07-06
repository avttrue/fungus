#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <kernel/kernel.h>

#include <QDialog>
#include <QFrame>

const QString LABEL_STYLE = "color: black; font-family: monospace; font: bold;";
const auto INFOPANEL_KEY_PROPERTY = "KeyValue";

class Cell;
class QGridLayout;
class QLabel;

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

private Q_SLOTS:
    void slotShowPoint();
    void slotCellAgeChanged(qint64 value);
    void slotCellStateChanged(int value);
    void slotCellGenerationChanged(qint64 value);

private:
    Cell* m_Cell;
    QGridLayout* glContent;
};

class DialogInfoPanel : public QFrame
{
    Q_OBJECT

    public:
    DialogInfoPanel(QWidget *parent, const QString& caption, const QString& value);
    void setValue(const QString& value);

    private:
    QLabel* m_LabelValue;
};

#endif // DIALOGCELLINFORMATION_H
