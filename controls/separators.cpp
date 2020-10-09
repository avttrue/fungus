#include "separators.h"

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

SeparatorH::SeparatorH(QWidget *parent): QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::HLine);
    setMidLineWidth(2);
    setLineWidth(1);
    setFocusPolicy(Qt::NoFocus);
}

SeparatorV::SeparatorV(QWidget *parent): QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::VLine);
    setMidLineWidth(2);
    setLineWidth(1);
    setFocusPolicy(Qt::NoFocus);
}


