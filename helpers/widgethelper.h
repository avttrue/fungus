#ifndef WIDGETHELPER_H
#define WIDGETHELPER_H

#include <QWidget>

class QToolBar;

/*!
 * поместить виджет по центру экрана
 */
void setWidgetToScreenCenter(QWidget* w);

/*!
 * поместить виджет по центру предка
 */
void setWidgetToParentCenter(QWidget* w);

/*!
 * найти окно по заголовку, если уже есть такое, то восстановить его
 */
bool findPreviousWindowCopy(const QString& caption);

void addToolBarAction(QToolBar *bar, QAction* action, const QString& name = "");

#endif // WIDGETHELPER_H
