#include "tooltipshortcut.h"

#include <QApplication>

/*
 * https://stackoverflow.com/a/42609206/12177714
 */

/* guesses a descriptive text from a text suited for a menu entry
   This is equivalent to QActions internal qt_strippedText() */
QString strippedActionText(QString text)
{
    text.remove(QString::fromUtf8("..."));
    for (int i = 0; i < text.size(); ++i)
        if (text.at(i) == QLatin1Char('&')) text.remove(i, 1);
    return text.trimmed();
}

/* Adds possible shortcut information to the tooltip of the action.
   This provides consistent behavior both with default and custom tooltips
   when used in combination with removeShortcutToToolTip() */
void addShortcutToToolTip(QAction *action)
{
    if (!action->shortcut().isEmpty())
    {
        auto tooltip = action->property("tooltipBackup").toString();
        if (tooltip.isEmpty())
        {
            tooltip = action->toolTip();
            if (tooltip != strippedActionText(action->text()))
                // action uses a custom tooltip. Backup so that we can restore it later.
                action->setProperty("tooltipBackup", action->toolTip());
        }
        auto shotrcut = action->shortcut().toString(QKeySequence::NativeText);
        QString s_plus('+');
        if(shotrcut.indexOf(s_plus) != shotrcut.size() - 1)
        {
            QString new_s_plus("&nbsp;+&nbsp;'");
            shotrcut.replace(shotrcut.indexOf(s_plus), s_plus.size(), new_s_plus);
            shotrcut.append("'");
        }

        auto shortcutTextColor = QApplication::palette().color(QPalette::ToolTipText);
        action->setToolTip(QString("<p style='white-space:pre'>"
                                   "%1&nbsp;&nbsp;&nbsp;["
                                   "<code style='color:%2; font-family:monospace'>"
                                   "%3</code>]</p>")
                           .arg(tooltip, shortcutTextColor.name(), shotrcut));
    }
}

/* Removes possible shortcut information from the tooltip of the action.
   This provides consistent behavior both with default and custom tooltips
   when used in combination with addShortcutToToolTip() */
void removeShortcutFromToolTip(QAction *action)
{
    action->setToolTip(action->property("tooltipBackup").toString());
    action->setProperty("tooltipBackup", QVariant());
}
