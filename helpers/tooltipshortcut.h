#ifndef TOOLTIPSHORTCUT_H
#define TOOLTIPSHORTCUT_H

#include <QAction>

QString strippedActionText(QString text);

void removeShortcutFromToolTip(QAction *action);
void addShortcutToToolTip(QAction *action);

#endif // TOOLTIPSHORTCUT_H
