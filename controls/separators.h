#ifndef SEPARATORS_H
#define SEPARATORS_H

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

#endif // SEPARATORS_H
