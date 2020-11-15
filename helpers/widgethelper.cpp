#include "widgethelper.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QToolBar>


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

void addToolBarAction(QToolBar* bar, QAction *action, const QString& name)
{
    if(!bar || !action) return;
    bar->addAction(action);

    if(name.isEmpty()) return;
    bar->widgetForAction(action)->setObjectName(name);
}
