#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QString &text, QWidget *parent): QLabel(text, parent)
{
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);
    setMargin(1);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    Q_EMIT signalClicked(this);
}

void ClickableLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
}

void ClickableLabel::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);
}
