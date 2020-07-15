#include "sceneview.h"
#include "scene.h"
#include "properties.h"
#include "sceneItem.h"
#include "graphicsviewzoomer.h"
#include "field/field.h"
#include "field/cell.h"

#include <QtMath>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>

SceneView::SceneView(QWidget *parent)
    :QGraphicsView(parent),
      m_Scene(nullptr)
{
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | DontSavePainterState);
    SetUpdateMode();
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::NoFocus);
    setCacheMode(QGraphicsView::CacheNone);
    setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_Zoomer = new GraphicsViewZoomer(this);
    m_Zoomer->SetZoomFactorBase(config->SceneScaleStep());
    m_Zoomer->setModifiers(config->SceneZoomModifier());

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "SceneView destroyed"; });
    qDebug() << "SceneView created";
}

Scene *SceneView::addScene(Field *filed)
{
    if(m_Scene) m_Scene->deleteLater();

    m_Scene = new Scene(this, filed);
    setScene(m_Scene);
    m_Scene->installEventFilter(this);

    return m_Scene;
}

bool SceneView::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_Scene)
    {
        if(event->type() != QEvent::GraphicsSceneMousePress &&
                event->type() != QEvent::GraphicsSceneMouseRelease &&
                event->type() != QEvent::GraphicsSceneMouseMove)
        { return false; }

        auto mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);

        if(mouseSceneEvent->scenePos().x() < 0 ||
                mouseSceneEvent->scenePos().x() >= m_Scene->size().width() ||
                mouseSceneEvent->scenePos().y() < 0 ||
                mouseSceneEvent->scenePos().y() >= m_Scene->size().height())
        {
            QToolTip::hideText();
            return false;
        }

        if(event->type() == QEvent::GraphicsSceneMouseMove &&
                mouseSceneEvent->modifiers() == config->SceneToolTipModifier())
        {
            auto field = m_Scene->getField();
            auto x = qFloor(mouseSceneEvent->scenePos().x() / config->SceneCellSize());
            auto y = qFloor(mouseSceneEvent->scenePos().y() / config->SceneCellSize());
            auto c = field->cells()->at(x).at(y);

            QToolTip::showText(QCursor::pos(), c->objectName(), this, rect());
        }

        if(event->type() == QEvent::GraphicsSceneMousePress)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qWarning() << __func__ << ": GraphicsSceneMousePress: target item not detected";
                return true;
            }
        }

        if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qWarning() << __func__ << ": GraphicsSceneMouseRelease: target item not detected";
                return true;
            }

            auto o = static_cast<SceneItem*>(item);

            // вывод сообщения об отсутствии SceneItem
            if(mouseSceneEvent->modifiers() == Qt::NoModifier &&
                    mouseSceneEvent->button() == Qt::LeftButton && !o)
            {
                qWarning() << __func__ << ": GraphicsSceneMouseRelease: SceneItem not detected";
                return true;
            }

            auto field = m_Scene->getField();
            auto x = qFloor(mouseSceneEvent->scenePos().x() / config->SceneCellSize());
            auto y = qFloor(mouseSceneEvent->scenePos().y() / config->SceneCellSize());
            auto c = field->cells()->at(x).at(y);

            if(mouseSceneEvent->button() == Qt::LeftButton)
            {
                m_Scene->selectCell(c);
                return true;
            }

            if(mouseSceneEvent->button() == Qt::RightButton)
            {
                m_Scene->selectCell(nullptr);
                return true;
            }
        }
    }
    return false;
}

void SceneView::SetUpdateMode()
{
    auto mode = config->SceneViewUpdateMode().toUpper();

    if(mode == SCENE_VIEW_UPDATE_MODES.at(0))
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    else if(mode == SCENE_VIEW_UPDATE_MODES.at(1))
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    else if(mode == SCENE_VIEW_UPDATE_MODES.at(2))
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    else
    {
        qCritical() << "Wrong settins value 'Scene/ViewUpdateMode'" <<  mode;
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }

    qDebug() << "SceneView update mode:" << viewportUpdateMode();
}

Scene *SceneView::getScene() const { return m_Scene; }
GraphicsViewZoomer *SceneView::zoomer() const { return m_Zoomer; }

