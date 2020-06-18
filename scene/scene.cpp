#include "scene.h"
#include "properties.h"
#include "sceneobject.h"
#include "sceneview.h"
#include "helper.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/fieldinformation.h"

#include <QDateTime>
#include <QDebug>

Scene::Scene(SceneView* parent, Field *field)
    : QGraphicsScene(parent),
      m_View(parent),
      m_Field(field),
      m_AverageDraw(0),
      m_StopAdvanse(false)
{
    m_Size = QSize(m_Field->width(), m_Field->height()) * config->SceneObjectSize();
    setObjectName(QString("SCENE[%1X%2]").
                  arg(QString::number(m_Field->width()), QString::number(m_Field->height())));

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
                           m_Field->width() * config->SceneObjectSize(),
                           m_Field->height() * config->SceneObjectSize(),
                           QPen(scenebordercolor, 0),
                           QBrush(scenecolor));

    setStickyFocus(true);

    QObject::connect(m_Field, &Field::signalCalculated, this, &Scene::slotAdvance, Qt::DirectConnection);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

SceneObject *Scene::addObject(int x, int y)
{
    auto c = m_Field->cells()->at(x).at(y);
    if(!c) c = m_Field->addCell(x, y);

    auto o = new SceneObject(this);
    o->setObjectName(QString("SCENE_OBJECT[%1.%2]").arg(QString::number(x), QString::number(y)));
    o->setIndex({x, y});
    o->setCell(c);
    c->setSceneObject(o);

    addItem(o);
    m_ObjectList.insert({x, y}, o);

    o->setPos(o->mapToParent(x * config->SceneObjectSize(), y * config->SceneObjectSize()));

    //qDebug() << "Object added:" << o->objectName() << ", count:" << m_ObjectList.count();
    return o;
}

void Scene::removeObject(SceneObject *object)
{
    m_ObjectList.remove({object->index().x(), object->index().y()});
    QObject::connect(object, &QObject::destroyed,
                     [=](){ qDebug() <<"SceneObject" << object->objectName() <<": destroyed"; });
    removeItem(object);
    object->deleteLater();
}

void Scene::removeObject(int x, int y)
{
    auto o = m_ObjectList.value({x, y});
    removeObject(o);
}

void Scene::fill()
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    m_ObjectList.reserve(m_Field->width() * m_Field->height());
    for(int h = 0; h < m_Field->height(); h++)
    {
        for(int w = 0; w < m_Field->width(); w++)
        {
            addObject(w, h);
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

SceneObject *Scene::focusedObject() const
{
    auto item = focusItem();
    if(!item) return nullptr;

    return static_cast<SceneObject*>(item->toGraphicsObject());
}

void Scene::slotAdvance(QSet<Cell*> cells)
{
    if(m_StopAdvanse) return;

    auto time = QDateTime::currentMSecsSinceEpoch();
    for(auto c: cells)
    {
        if(m_StopAdvanse) return;

        auto o = c->getSceneObject();
        if(!o) addObject(c->getIndex().x(), c->getIndex().y());

        o->setUpdate(true);
    }

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=]()
    {
        QObject::disconnect(*conn);
        auto dt = QDateTime::currentMSecsSinceEpoch() - time;
        m_AverageDraw = calcAverage(m_AverageDraw, m_Field->getInformation()->getAge(), dt);
        m_Field->setWaitScene(false);
        Q_EMIT signalAdvansedTime(m_AverageDraw);
    };
    *conn = QObject::connect(this, &Scene::changed, func);
    advance();
}

QGraphicsRectItem *Scene::borderRect() const { return m_BorderRect; }
QHash<QPair<int, int>, SceneObject*>* Scene::objectList() const { return const_cast<QHash<QPair<int, int>, SceneObject*>*>(&m_ObjectList); }
QSize Scene::size() const { return m_Size; }
QColor Scene::getBackgroundColor() const { return m_BackgroundColor; }
SceneView *Scene::getView() const { return m_View; }
Field *Scene::getField() const { return m_Field; }
void Scene::StopAdvanse() { m_StopAdvanse = true; }
