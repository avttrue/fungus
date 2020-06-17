#include "cell.h"
#include "cellinformation.h"
#include "cellrule.h"
#include "field.h"

#include <QDebug>
#include <scene/sceneobject.h>

Cell::Cell(QObject *parent)
    : QObject(parent),
    m_Field(nullptr),
    m_Index({-1, -1}),
    m_SceneObject(nullptr)
{
    m_Information = new CellInformation(this);
    //QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "Cell"<< objectName() <<"destroyed"; });
    //qDebug() << "Cell created";
}

void Cell::clear()
{
    m_Information->setAge(0);
    m_Information->setState(Kernel::CellState::Dead);
    m_Information->setGeneration(0);
}

SceneObject *Cell::getSceneObject() const { return m_SceneObject; }
void Cell::setSceneObject(SceneObject *value) { m_SceneObject = value; }
QPoint Cell::getIndex() { return m_Index; }
void Cell::setIndex(const QPoint &value) { m_Index = value; }
CellInformation *Cell::getInformation() { return m_Information; }
Field *Cell::getField() const { return m_Field; }
void Cell::setField(Field *value) { m_Field = value; }

