#ifndef CONTROLS_H
#define CONTROLS_H

#include <QFrame>
#include <QLabel>
#include <QWidgetAction>

const QString MENU_CAPTION_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                   "stop: 0 transparent, "
                                   "stop: 0.5 silver, "
                                   "stop: 0.5 silver, "
                                   "stop: 1 transparent); "
                                   "padding: 0px 1px 0px 1px; border-radius: 3px;"
                                   "color: black; font-family: monospace; font: bold;";

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
    ClickableLabel(const QString &text, QWidget *parent = nullptr);
protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

Q_SIGNALS:
    void signalClicked(ClickableLabel* label);
};

/*!
 * \brief MenuCaption - заголовок для QMenu
 */
class MenuCaption : public QWidgetAction
{
public:
    explicit MenuCaption(QObject* parent, const QString& caption);
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
