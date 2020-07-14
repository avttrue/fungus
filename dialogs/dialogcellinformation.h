#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>
#include <QFrame>

const QString DCI_LABEL_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                "stop: 0 transparent, "
                                "stop: 0.5 silver, "
                                "stop: 0.5 silver, "
                                "stop: 1 transparent); "
                                "padding: 0px 1px 0px 1px; border-radius: 3px;"
                                "color: black; font-family: monospace; font: bold;";
const auto DCI_INFOPANEL_KEY_PROPERTY = "KeyValue";

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
