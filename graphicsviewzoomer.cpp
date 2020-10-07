#include "graphicsviewzoomer.h"

#include <QDebug>
#include <QApplication>
#include <QGraphicsView>
#include <QMouseEvent>
#include <qmath.h>

GraphicsViewZoomer::GraphicsViewZoomer(QGraphicsView* view)
    : QObject(view),
      m_View(view),
      m_Modifiers(MODIFIER),
      m_ZoomFactorBase(ZOOM_FACTOR_BASE),
      m_CurrentZoom(1.0)
{
    setObjectName("GraphicsViewZoomer");

    m_View->viewport()->installEventFilter(this);
    m_View->setMouseTracking(true);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void GraphicsViewZoomer::Zoom(qreal factor, bool centered)
{
    qDebug() << __func__;

    if(!m_View->scene())
    {
        qCritical() << ": Scene not created";
        return;
    }

    if(factor - ZOOM_FACTOR_RESET == 0.0) // reset
    {
        m_View->resetTransform();
        m_CurrentZoom = 1.0;
        qDebug() << "Scene zoom resetted";
    }
    else
    {
        m_View->scale(factor, factor);
        QPointF center;

        if(centered)
            center = QPointF(m_View->scene()->width(), m_View->scene()->height()) / 2.0;
        else
        {
            m_View->centerOn(m_TargetScenePos);
            auto delta = QPointF(m_View->viewport()->width(), m_View->viewport()->height()) / 2.0;
            center = m_View->mapFromScene(m_TargetScenePos) - m_TargetViewportPos + delta;
            center = m_View->mapToScene(center.toPoint());
        }

        m_View->centerOn(center);
        m_CurrentZoom *= factor;
        qDebug() << "Scene zoom center:" << center;
    }

    qDebug() << "Scene zoom:" << m_CurrentZoom;
    Q_EMIT signalZoomed(m_CurrentZoom);
}

void GraphicsViewZoomer::ZoomFitToView()
{
    qDebug() << __func__;
    auto s = m_View->scene();
    if(!s) return;

    Zoom(-1, false);

    m_CurrentZoom = qMin(m_View->width() / s->width(), m_View->height() / s->height()) + 2 * (1 - ZOOM_FACTOR_BASE);

    m_View->scale(m_CurrentZoom, m_CurrentZoom);

    qDebug() << "Scene zoom:" << m_CurrentZoom;
    Q_EMIT signalZoomed(m_CurrentZoom);
}

bool GraphicsViewZoomer::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);

    if(!m_View->scene()) return false;

    if(event->type() == QEvent::MouseMove)
    {
        auto mouseevent = static_cast<QMouseEvent*>(event);
        auto delta = m_TargetViewportPos - mouseevent->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            m_TargetViewportPos = mouseevent->pos();
            m_TargetScenePos = m_View->mapToScene(mouseevent->pos());
        }
    }
    else if(event->type() == QEvent::Wheel)
    {
        auto wheelevent = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == m_Modifiers)
        {
            auto angle = wheelevent->angleDelta().y();
            auto factor = qPow(m_ZoomFactorBase, angle);
            Zoom(factor);
            return true;
        }
    }
    return false;
}

void GraphicsViewZoomer::setModifiers(Qt::KeyboardModifiers value) { m_Modifiers = value; }
void GraphicsViewZoomer::setZoomFactorBase(qreal value) { m_ZoomFactorBase = value; }
qreal GraphicsViewZoomer::CurrentZoom() const { return m_CurrentZoom; }
