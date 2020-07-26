#ifndef DIALOGINFOPANEL_H
#define DIALOGINFOPANEL_H

#include <QFrame>

const auto REAL_VALUE_PRECISION = 3;
const QString DCI_LABEL_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                "stop: 0 transparent, "
                                "stop: 0.5 silver, "
                                "stop: 0.5 silver, "
                                "stop: 1 transparent); "
                                "padding: 0px 1px 0px 1px; border-radius: 3px;"
                                "color: black; font-family: monospace; font: bold;";
const auto DCI_INFOPANEL_KEY_PROPERTY = "KeyValue";

class QLabel;

class DialogInfoPanel : public QFrame
{
    Q_OBJECT

public:
    DialogInfoPanel(QWidget *parent, const QString& caption, const QVariant &value);
    void setValue(const QVariant &value);

private:
    QLabel* m_LabelValue;
};

#endif // DIALOGINFOPANEL_H
