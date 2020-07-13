#include "sceneItem.h"
#include "scene.h"
#include "properties.h"
#include "field/field.h"

#include <QDebug>
#include <QPainter>

SceneItem::SceneItem(Scene* scene, QGraphicsItem* parent)
    : QGraphicsItem(parent),
      m_Scene(scene)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    m_Pixmap = QPixmap(scene->getField()->width() * config->SceneCellSize(),
                       scene->getField()->height() * config->SceneCellSize());

    m_Pixmap.fill(config->SceneCellDeadColor());
}

QPainterPath SceneItem::shape() const
{
    QPainterPath path;

    path.addRect(0, 0, m_Pixmap.width(), m_Pixmap.height());

    return path;
}

void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0, 0, m_Pixmap);
}

void SceneItem::advance(int step)
{
    if (!step) return;

    update();
}

QPixmap *SceneItem::getPixmap() { return &m_Pixmap; }
Scene *SceneItem::getScene() const { return m_Scene; }
QRectF SceneItem::boundingRect() const { return QRectF(0, 0, m_Pixmap.width(), m_Pixmap.height()); }
