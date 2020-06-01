#include "scene.h"
#include "properties.h"
#include "sceneobject.h"
#include "field/field.h"
#include "field/cell.h"

#include <QDateTime>
#include <QDebug>

Scene::Scene(QObject *parent, Field *field)
    : QGraphicsScene(parent),
    m_Field(field)
{
    m_Size = QSize(m_Field->Width(), m_Field->Height()) * config->SceneObjectSize();
    setObjectName(QString("SCENE[%1X%2]").
                  arg(QString::number(m_Field->Width()), QString::number(m_Field->Height())));

    if(config->SceneBspTreeIndex())
    {
        setItemIndexMethod(QGraphicsScene::BspTreeIndex);
        setBspTreeDepth(config->SceneBspTreeDepth());
    }
    else setItemIndexMethod(QGraphicsScene::NoIndex);

    if(QColor::isValidColor(config->SceneBgColor()))
        setBackgroundColor(QColor(config->SceneBgColor()));
    else setBackgroundColor(QColor(SCENE_BG_COLOR));

    QColor scenecolor;
    if(QColor::isValidColor(config->SceneColor()))
        scenecolor = QColor(config->SceneColor());
    else scenecolor = QColor(SCENE_COLOR);

    QColor scenebordercolor;
    if(QColor::isValidColor(config->SceneBorderColor()))
        scenebordercolor = QColor(config->SceneBorderColor());
    else scenebordercolor = QColor(SCENE_BORDER_COLOR);

    m_BorderRect = addRect(0, 0,
                           m_Field->Width() * config->SceneObjectSize(),
                           m_Field->Height() * config->SceneObjectSize(),
                           QPen(scenebordercolor, 0),
                           QBrush(scenecolor));

    setStickyFocus(true);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

SceneObject *Scene::AddObject(int x, int y)
{
    auto c = m_Field->Cells()->at(x).at(y);
    if(!c) c = m_Field->addCell(x, y);

    auto o = new SceneObject;
    o->setObjectName(QString("SCENE_OBJECT[%1.%2]").arg(QString::number(x), QString::number(y)));
    o->setIndex({x, y});
    o->setCell(c);
    c->setSceneObject(o);

    addItem(o);
    m_ObjectList.insert({x, y}, o);

    o->setPos(o->mapToParent(x * config->SceneObjectSize(), y * config->SceneObjectSize()));

    //qDebug() << "Object added:" << o->objectName() << ", count:" << m_ObjectList.count();
    Q_EMIT signalObjectAdded(o);
    return o;
}

void Scene::RemoveObject(SceneObject *object)
{
    m_ObjectList.remove({object->Index().x(), object->Index().y()});
    QObject::connect(object, &QObject::destroyed, [=](){ qDebug() <<"SceneObject" << object->objectName() <<": destroyed"; });
    removeItem(object);
    object->deleteLater();
    Q_EMIT signalObjectRemoved();
}

void Scene::RemoveObject(int x, int y)
{
    auto o = m_ObjectList.value({x, y});
    RemoveObject(o);
}

void Scene::Fill()
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    m_ObjectList.reserve(m_Field->Width() * m_Field->Height());
    for(int h = 0; h < m_Field->Height(); h++)
    {
        for(int w = 0; w < m_Field->Width(); w++)
        {
            AddObject(w, h);
        }
        Q_EMIT signalProgress(h + 1);
    }
    qDebug() << "Scene" << objectName() << "filled in"
             << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

void Scene::setBackgroundColor(const QColor &value)
{
    m_BackgroundColor = value;
    setBackgroundBrush(m_BackgroundColor);
}

SceneObject *Scene::FocusedObject()
{
    auto item = focusItem();
    if(!item) return nullptr;

    return static_cast<SceneObject*>(item->toGraphicsObject());
}

QGraphicsRectItem *Scene::BorderRect() const { return m_BorderRect; }
QHash<QPair<int, int>, SceneObject *> *Scene::ObjectList() { return &m_ObjectList; }
QSize Scene::Size() const { return m_Size; }
QColor Scene::BackgroundColor() const { return m_BackgroundColor; }

