#ifndef CONTROLS_H
#define CONTROLS_H

#include <QFrame>

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
