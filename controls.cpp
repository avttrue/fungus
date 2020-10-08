#include "controls.h"

#include <QKeyEvent>

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{ setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); }

SeparatorH::SeparatorH(QWidget *parent):
    QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::HLine);
    setMidLineWidth(2);
    setLineWidth(1);
    setFocusPolicy(Qt::NoFocus);
}

SeparatorV::SeparatorV(QWidget *parent):
    QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::VLine);
    setMidLineWidth(2);
    setLineWidth(1);
    setFocusPolicy(Qt::NoFocus);
}

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

MenuCaption::MenuCaption(QObject *parent, const QString &caption)
    : QWidgetAction(parent)
{
    auto labelCaption = new QLabel(QString("<center><h3>%1</h3></center>").arg(caption));
    labelCaption->setStyleSheet(MENU_CAPTION_STYLE);
    labelCaption->setAlignment(Qt::AlignHCenter);
    setDefaultWidget(labelCaption);
}

