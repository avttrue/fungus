#include "sceneobject.h"
#include "properties.h"
#include "field/cell.h"
#include "field/cellinformation.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

SceneObject::SceneObject(QGraphicsItem* parent)
    : QGraphicsObject(parent),
    m_Size(config->SceneObjectSize()),
    m_Index(-1, -1),
    m_Cell(nullptr),
    m_Update(true)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    //QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "SceneObject" << objectName() <<" destroyed"; });
    //qDebug() << "SceneObject created";
}

QPainterPath SceneObject::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, m_Size, m_Size);
    return path;
}

void SceneObject::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem*,
                        QWidget*)
{
    QColor color;
    if(!m_Cell->getInformation()->getAlive()) color = QColor(config->SceneObjectDeadColor());
    else if(config->SceneObjectAgeIndicate())
    {
        if(m_Cell->getInformation()->getAge() == 0) color = QColor(config->SceneObjectAlive0Color());
        else if(m_Cell->getInformation()->getAge() == 1) color = QColor(config->SceneObjectAlive1Color());
        else if(m_Cell->getInformation()->getAge() == 2) color = QColor(config->SceneObjectAlive2Color());
        else if(m_Cell->getInformation()->getAge() == 3) color = QColor(config->SceneObjectAlive3Color());
        else if(m_Cell->getInformation()->getAge() == 4) color = QColor(config->SceneObjectAlive4Color());
        else if(m_Cell->getInformation()->getAge() == 5) color = QColor(config->SceneObjectAlive5Color());
        else if(m_Cell->getInformation()->getAge() == 6) color = QColor(config->SceneObjectAlive6Color());
        else if(m_Cell->getInformation()->getAge() == 7) color = QColor(config->SceneObjectAlive7Color());
        else if(m_Cell->getInformation()->getAge() == 8) color = QColor(config->SceneObjectAlive8Color());
        else if(m_Cell->getInformation()->getAge() >= 9) color = QColor(config->SceneObjectAlive9Color());
    }
    else color = QColor(config->SceneObjectAlive0Color());

    painter->fillRect(0, 0, m_Size, m_Size, color);
}

void SceneObject::advance(int step)
{
    if (!step) return;

    if(!m_Update) return;

    m_Update = false;
    update();
}

void SceneObject::setUpdate(bool value) { m_Update = value; }
QRectF SceneObject::boundingRect() const { return QRectF(0, 0, m_Size, m_Size); }
Cell *SceneObject::getCell() const { return m_Cell; }
void SceneObject::setCell(Cell *Cell) { m_Cell = Cell; }
void SceneObject::setIndex(const QPoint &value) { m_Index = value; }
QPoint SceneObject::index() const { return m_Index; }
int SceneObject::size() const { return m_Size; }

