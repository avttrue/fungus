#include "sceneobject.h"
#include "scene.h"
#include "properties.h"
#include "field/cell.h"
#include "field/field.h"
#include "field/cellrule.h"
#include "field/cellinformation.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

SceneObject::SceneObject(Scene* scene, QGraphicsItem* parent)
    : QGraphicsObject(parent),
      m_Scene(scene),
      m_Cell(nullptr),
      m_Size(config->SceneObjectSize()),
      m_Index(-1, -1),
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

    if(isSelected()) color = QColor(config->SceneSelectColor());

    else if(m_Cell->getInformation()->getState() == Kernel::CellState::Dead)
        color = QColor(config->SceneObjectDeadColor());

    else if(m_Cell->getInformation()->getState() == Kernel::CellState::Cursed)
        color = QColor(config->SceneObjectCurseColor());

    else if(config->SceneObjectAgeIndicate())
    {
        color = m_Cell->getField()->getRule()->getColorAlive();
        // if(m_Cell->getInformation()->getAge() == 0)
        // TODO: изменение цвета по восрасту ячейки
    }

    else color = m_Cell->getField()->getRule()->getColorAlive();

    painter->fillPath(shape(), color);
}

void SceneObject::advance(int step)
{
    if (!step) return;

    if(!m_Update) return;

    m_Update = false;
    update();
}

Scene *SceneObject::getScene() const { return m_Scene; }
void SceneObject::setUpdate(bool value) { m_Update = value; }
QRectF SceneObject::boundingRect() const { return QRectF(0, 0, m_Size, m_Size); }
Cell *SceneObject::getCell() const { return m_Cell; }
void SceneObject::setCell(Cell *Cell) { m_Cell = Cell; }
void SceneObject::setIndex(const QPoint &value) { m_Index = value; }
QPoint SceneObject::index() const { return m_Index; }
int SceneObject::size() const { return m_Size; }

