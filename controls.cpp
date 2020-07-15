#include "controls.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QApplication>

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

void setWidgetToScreenCenter(QWidget* w)
{
    QGuiApplication::screens();
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    QPoint center = rect.center();

    center.setX(center.x() - (w->width()/2));
    center.setY(center.y() - (w->height()/2));
    w->move(center);
}

void setWidgetToParentCenter(QWidget* w)
{
    QWidget* p = w->parentWidget();
    if(p == nullptr) return;
    QRect rect = p->geometry();
    QPoint center = rect.center();
    center.setX(center.x() - (w->width()/2));
    center.setY(center.y() - (w->height()/2));
    w->move(center);
}

bool findPreviousWindowCopy(const QString& caption)
{
    for (auto w: QApplication::topLevelWidgets())
    {
        if (!w) continue;

        if(w->windowTitle() == caption)
        {
            w->showNormal();
            w->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}
