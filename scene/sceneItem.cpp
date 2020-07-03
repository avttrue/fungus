#include "sceneItem.h"
#include "scene.h"
#include "properties.h"
#include "field/cell.h"
#include "field/field.h"
#include "field/cellrule.h"
#include "field/cellinformation.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
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

    if(!m_Pixmap)
        qDebug() << "shape: !m_Pixmap";
    else
        path.addRect(0, 0, m_Pixmap.width(), m_Pixmap.height());

    return path;
}

void SceneItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!m_Pixmap)
        qDebug() << "paint: !m_Pixmap";
    else
        painter->drawPixmap(0, 0, m_Pixmap);
}

void SceneItem::advance(int step)
{
    if (step == 0) return;

    else if(step == 1)
    {
        update();
    }
}

void SceneItem::setPixmap(QPixmap pixmap)
{
    m_Pixmap.swap(pixmap);
}

Scene *SceneItem::getScene() const { return m_Scene; }
QRectF SceneItem::boundingRect() const { return QRectF(0, 0, m_Pixmap.width(), m_Pixmap.height()); }
