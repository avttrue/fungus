#include "sceneview.h"
#include "scene.h"
#include "properties.h"
#include "sceneitem.h"
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
    setObjectName("SceneView");
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | DontSavePainterState);
    SetUpdateMode();
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::NoFocus);
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_Zoomer = new GraphicsViewZoomer(this);
    m_Zoomer->SetZoomFactorBase(config->SceneScaleStep());
    m_Zoomer->setModifiers(config->SceneZoomModifier());

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
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
            auto c = getCell(mouseSceneEvent->scenePos().x(), mouseSceneEvent->scenePos().y());
            QToolTip::showText(QCursor::pos(), c->objectName(), this, rect());
            return true;
        }

        if(event->type() == QEvent::GraphicsSceneMousePress)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qWarning()<< __func__ << ": GraphicsSceneMousePress: target item not detected";
                return true;
            }
        }

        if(event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            if(mouseSceneEvent->modifiers() == Qt::NoModifier &&
                    mouseSceneEvent->button() == Qt::LeftButton)
            {
                auto c = getCell(mouseSceneEvent->scenePos().x(), mouseSceneEvent->scenePos().y());
                m_Scene->selectCell(c);
                return true;
            }

            if(mouseSceneEvent->modifiers() == config->SceneMultiselModifier() &&
                    mouseSceneEvent->button() == Qt::LeftButton)
            {
                auto c = getCell(mouseSceneEvent->scenePos().x(), mouseSceneEvent->scenePos().y());
                m_Scene->MultiselectCell(c);
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

Cell *SceneView::getCell(qreal x, qreal y)
{
    if(!m_Scene) return nullptr;
    auto field = m_Scene->getField();
    if(!field) return nullptr;

    auto cx = qFloor(x / config->SceneCellSize());
    auto cy = qFloor(y / config->SceneCellSize());
    return field->cells()->at(cx).at(cy);
}

void SceneView::SetUpdateMode()
{
    auto mode = config->SceneViewUpdateMode().toUpper();
    QGraphicsView::ViewportUpdateMode viewport_update_mode = QGraphicsView::FullViewportUpdate;

    if(mode == SCENE_VIEW_UPDATE_MODES.at(0)) viewport_update_mode = QGraphicsView::SmartViewportUpdate;
    else if(mode == SCENE_VIEW_UPDATE_MODES.at(1)) viewport_update_mode = QGraphicsView::MinimalViewportUpdate;
    else if(mode == SCENE_VIEW_UPDATE_MODES.at(2)) viewport_update_mode = QGraphicsView::FullViewportUpdate;
    else qCritical() << "Wrong settins value 'Scene/ViewUpdateMode'" <<  mode;

    setViewportUpdateMode(viewport_update_mode);
    qDebug() << objectName() << "update mode:" << viewportUpdateMode();
}

Scene *SceneView::getScene() const { return m_Scene; }
GraphicsViewZoomer *SceneView::zoomer() const { return m_Zoomer; }

