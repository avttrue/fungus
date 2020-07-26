#ifndef CONTROLS_H
#define CONTROLS_H

#include <QFrame>
#include <QLabel>

class WidgetSpacer: public QWidget
{

public:
    explicit WidgetSpacer(QWidget* parent = nullptr);
};

class SeparatorH:  public QFrame
{
public:
    explicit SeparatorH(QWidget* parent = nullptr);
};

class SeparatorV:  public QFrame
{
public:
    explicit SeparatorV(QWidget* parent = nullptr);
};

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(const QString &text, QWidget *parent = nullptr): QLabel(text, parent){}
protected:
    void mouseReleaseEvent(QMouseEvent* event);

Q_SIGNALS:
    void signalClicked(ClickableLabel* label);
};

/*!
 * поместить виджет по центру экрана
 */
void setWidgetToScreenCenter(QWidget* w);

/*!
 * поместить виджет по центру предка
 */
void setWidgetToParentCenter(QWidget* w);

bool findPreviousWindowCopy(const QString& caption);

#endif // CONTROLS_H
