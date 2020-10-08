#include "graphicsviewzoomer.h"

#include <QDebug>
#include <QApplication>
#include <QGraphicsView>
#include <QMouseEvent>
#include <qmath.h>

GraphicsViewZoomer::GraphicsViewZoomer(QGraphicsView* view,
                                       Qt::KeyboardModifiers key_modifier,
                                       qreal zoom_factor_base)
    : QObject(view),
      m_View(view),
      m_KeyModifier(key_modifier),
      m_ZoomFactorBase(zoom_factor_base),
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
    { qCritical() << ": Scene not created"; return; }

    QPointF center;
    m_View->scale(factor, factor);

    if(centered)
        center = QPointF(m_View->scene()->width(), m_View->scene()->height()) / 2.0;
    else
    {
        m_View->centerOn(m_TargetScenePos);
        auto delta = QPointF(m_View->viewport()->width(), m_View->viewport()->height()) / 2.0;
        center = m_View->mapFromScene(m_TargetScenePos) - m_TargetViewportPos + delta;
        center = m_View->mapToScene(center.toPoint());
    }

    m_CurrentZoom *= factor;
    m_View->centerOn(center);

    qDebug() << "Scene zoom:" << m_CurrentZoom << "; center:" << center;
    Q_EMIT signalZoomed(m_CurrentZoom);
}

void GraphicsViewZoomer::resetZoom()
{
    qDebug() << __func__;

    if(!m_View->scene())
    { qCritical() << ": Scene not created"; return; }

    m_View->resetTransform();
    m_CurrentZoom = 1.0;
    auto center = QPointF(m_View->scene()->width(), m_View->scene()->height()) / 2.0;
    m_View->centerOn(center);

    qDebug() << "Scene zoom resetted:" << m_CurrentZoom << "; center:" << center;
    Q_EMIT signalZoomed(m_CurrentZoom);
}

void GraphicsViewZoomer::ZoomFitToView()
{
    qDebug() << __func__;
    auto s = m_View->scene();
    if(!s) return;

    resetZoom();

    auto factor = 1 / (m_ZoomFactorBase + 100 * (m_ZoomFactorBase - 1));
    m_CurrentZoom = (qMin(m_View->width() / s->width(), m_View->height() / s->height())) * factor;

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
        if (QApplication::keyboardModifiers() == m_KeyModifier)
        {
            auto angle = wheelevent->angleDelta().y();
            auto factor = qPow(m_ZoomFactorBase, angle);
            Zoom(factor);
            return true;
        }
    }
    return false;
}

void GraphicsViewZoomer::setKeyModifier(Qt::KeyboardModifiers value) { m_KeyModifier = value; }
void GraphicsViewZoomer::setZoomFactorBase(qreal value) { m_ZoomFactorBase = value; }
qreal GraphicsViewZoomer::currentZoom() const { return m_CurrentZoom; }
