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
      m_Scene(scene),
      m_Cell(nullptr),
      m_Size(config->SceneCellSize()),
      m_Index(-1, -1),
      m_Update(true)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    //QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "SceneObject" << objectName() <<" destroyed"; });
    //qDebug() << "SceneObject created";
}

QPainterPath SceneItem::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, m_Size, m_Size);
    return path;
}

void SceneItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor color;

    if(isSelected()) color = QColor(config->SceneSelectColor());

    // dead
    else if(m_Cell->getInformation()->getState() == Kernel::CellState::Dead)
        color = QColor(config->SceneObjectDeadColor());

    // cursed
    else if(m_Cell->getInformation()->getState() == Kernel::CellState::Cursed)
        color = QColor(config->SceneObjectCurseColor());

    // alive
    else if(config->SceneObjectAgeIndicate())
    {
        color = m_Cell->getField()->getRule()->getColorAlive();
        // if(m_Cell->getInformation()->getAge() == 0)
        // TODO: изменение цвета по восрасту ячейки
    }

    else color = m_Cell->getField()->getRule()->getColorAlive();

    painter->fillPath(shape(), color);
}

void SceneItem::advance(int step)
{
    if (!step) return;

    if(!m_Update) return;

    m_Update = false;
    update();
}

QString SceneItem::getName() const
{
    return m_Name;
}

void SceneItem::setName(const QString &Name)
{
    m_Name = Name;
}

Scene *SceneItem::getScene() const { return m_Scene; }
void SceneItem::setUpdate(bool value) { m_Update = value; }
QRectF SceneItem::boundingRect() const { return QRectF(0, 0, m_Size, m_Size); }
Cell *SceneItem::getCell() const { return m_Cell; }
void SceneItem::setCell(Cell *Cell) { m_Cell = Cell; }
void SceneItem::setIndex(const QPoint &value) { m_Index = value; }
QPoint SceneItem::index() const { return m_Index; }
int SceneItem::size() const { return m_Size; }

