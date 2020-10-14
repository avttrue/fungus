#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QFrame>

const QString IP_LABEL_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                               "stop: 0 white, stop: 0.5 silver, "
                               "stop: 0.5 silver,  stop: 1 white); "
                               "padding: 0px 1px 0px 1px; border-radius: 3px;"
                               "color: black; font-family: monospace; font: bold;";
const auto IP_INFOPANEL_KEY_PROPERTY = "KeyValue";

class QLabel;

class InfoPanel : public QFrame
{
    Q_OBJECT
public:
    InfoPanel(QWidget *parent, const QString& caption, const QVariant &value);
    void setValue(const QVariant &value);

private:
    QLabel* m_LabelValue;
};

#endif // INFOPANEL_H
