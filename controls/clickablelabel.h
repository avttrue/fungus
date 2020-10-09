#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(const QString &text, QWidget *parent = nullptr);
protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

Q_SIGNALS:
    void signalClicked(ClickableLabel* label);
};

#endif // CLICKABLELABEL_H
