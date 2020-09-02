#include "sceneitem.h"
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

    auto glw = config->SceneGridLineWidth();
    auto cs = config->SceneCellSize();
    m_Rect = QRect(0, 0,
                   scene->getField()->width() * (cs + glw) + glw,
                   scene->getField()->height() * (cs + glw) + glw);

    m_Pixmap = QPixmap(m_Rect.size());
    m_Pixmap.fill(config->SceneCellDeadColor());
    m_Buffer = QPixmap(m_Pixmap.size());
}

void SceneItem::advance(int phase)
{
    if(!phase) { m_Pixmap.swap(m_Buffer); return; }

    update();
    m_Scene->update(); // NOTE: ? костыль для ситуации остановки обновления SceneItem
}

void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0, 0, m_Pixmap);
}

QPixmap* SceneItem::getPixmap() { return &m_Pixmap; }
QRect SceneItem::rect() const { return m_Rect; }
QPixmap* SceneItem::getBuffer() { return &m_Buffer; }
Scene* SceneItem::getScene() const { return m_Scene; }
QRectF SceneItem::boundingRect() const { return m_Rect; }
