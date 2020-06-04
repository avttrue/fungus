#include "cell.h"
#include "cellinformation.h"
#include "cellrule.h"
#include "field.h"

#include <QDebug>
#include <scene/sceneobject.h>

Cell::Cell(Field *parent, CellRule *rule)
    : QObject(parent),
    m_Field(parent),
    m_Rule(rule),
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
    m_Information->setAlive(false);
    m_Information->setGeneration(0);
}

SceneObject *Cell::getSceneObject() const { return m_SceneObject; }
void Cell::setSceneObject(SceneObject *value) { m_SceneObject = value; }
QPoint Cell::getIndex() const { return m_Index; }
void Cell::setIndex(const QPoint &value) { m_Index = value; }
CellRule *Cell::getRule() const { return m_Rule; }
void Cell::setRule(CellRule *value) { m_Rule = value; }
CellInformation *Cell::getInformation() { return m_Information; }
Field *Cell::getField() const { return m_Field; }

