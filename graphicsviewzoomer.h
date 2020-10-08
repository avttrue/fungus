#ifndef GRAPHICSVIEWZOOMER_H
#define GRAPHICSVIEWZOOMER_H

#include <QObject>
#include <QPointF>

class QGraphicsView;

/*!
 * класс для зума сцены QGraphicsView
 * выполнено по материалам: https://stackoverflow.com/a/19114517/12177714
 */
class GraphicsViewZoomer : public QObject
{
    Q_OBJECT
public:
    GraphicsViewZoomer(QGraphicsView* view,
                       Qt::KeyboardModifiers key_modifier,
                       qreal zoom_factor_base);
    void Zoom(qreal factor, bool centered = false);
    void resetZoom();
    void ZoomFitToView();
    void setKeyModifier(Qt::KeyboardModifiers value);
    void setZoomFactorBase(qreal value);
    qreal currentZoom() const;

private:
    QGraphicsView* m_View;
    Qt::KeyboardModifiers m_KeyModifier;
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
