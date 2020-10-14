#ifndef MENUCAPTION_H
#define MENUCAPTION_H

#include <QWidgetAction>

const QString MENU_CAPTION_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                   "stop: 0 Silver, stop: 0.5 PowderBlue, "
                                   "stop: 0.5 PowderBlue, stop: 1 Silver); "
                                   "border: 1px solid silver; "
                                   "padding: 0px 1px 0px 1px; "
                                   "color: black; font-family: monospace; font: bold; ";

/*!
 * \brief MenuCaption - заголовок для QMenu
 */
class MenuCaption : public QWidgetAction
{
public:
    explicit MenuCaption(QObject* parent, const QString& caption);
};

#endif // MENUCAPTION_H
