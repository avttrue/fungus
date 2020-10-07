#ifndef GRAPHICSVIEWZOOMER_H
#define GRAPHICSVIEWZOOMER_H

#include <QObject>
#include <QPointF>

const auto GVZ_ZOOM_FACTOR_BASE = 1.001;
const auto GVZ_ZOOM_FACTOR_RESET = -1;
const auto GVZ_MODIFIER = Qt::ControlModifier;

class QGraphicsView;

/*!
 * класс для зума сцены QGraphicsView
 * выполнено по материалам: https://stackoverflow.com/a/19114517/12177714
 */
class GraphicsViewZoomer : public QObject
{
    Q_OBJECT
public:
    GraphicsViewZoomer(QGraphicsView* view);
    void Zoom(qreal factor, bool centered = false);
    void ZoomFitToView();
    void setModifiers(Qt::KeyboardModifiers value);
    void setZoomFactorBase(qreal value);
    qreal CurrentZoom() const;

private:
    QGraphicsView* m_View;
    Qt::KeyboardModifiers m_Modifiers;
    qreal m_ZoomFactorBase;
    QPointF m_TargetScenePos;
    QPointF m_TargetViewportPos;
    qreal m_CurrentZoom;

protected:
    bool eventFilter(QObject* object, QEvent* event);

Q_SIGNALS:
    void signalZoomed(qreal value);

};

#endif // GRAPHICSVIEWZOOMER_H
