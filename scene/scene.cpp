#include "scene.h"
#include "properties.h"
#include "sceneItem.h"
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
    m_Size = QSize(m_Field->width(), m_Field->height()) * config->SceneCellSize();
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
                           m_Field->width() * config->SceneCellSize(),
                           m_Field->height() * config->SceneCellSize(),
                           QPen(scenebordercolor, 0),
                           QBrush(scenecolor));

    setStickyFocus(true);

    QObject::connect(m_Field, &Field::signalCalculated, this, &Scene::slotAdvance, Qt::DirectConnection);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

SceneItem *Scene::addSceneItem(int x, int y)
{
    auto c = m_Field->cells()->at(x).at(y);
    if(!c) c = m_Field->addCell(x, y); // если автозаполнение выключено

    auto o = new SceneItem(this);
    o->setName(QString("SCENE_ITEM[%1.%2]").arg(QString::number(x), QString::number(y)));
    o->setIndex({x, y});
    o->setCell(c);
    c->setSceneObject(o);

    addItem(o);
    m_ItemsList.insert({x, y}, o);

    o->setPos(o->mapToParent(x * config->SceneCellSize(), y * config->SceneCellSize()));

    return o;
}

void Scene::fill()
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    m_ItemsList.reserve(m_Field->width() * m_Field->height());
    for(int h = 0; h < m_Field->height(); h++)
    {
        for(int w = 0; w < m_Field->width(); w++)
        {
            addSceneItem(w, h);
        }
        Q_EMIT signalProgress(h + 1);
    }
    qDebug() << "Scene" << objectName() << "filled in" << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

void Scene::setBackgroundColor(const QColor &value)
{
    m_BackgroundColor = value;
    setBackgroundBrush(m_BackgroundColor);
}

SceneItem *Scene::focusedItem() const
{
    auto item = focusItem();
    if(!item) return nullptr;

    return static_cast<SceneItem*>(item);
}

void Scene::slotAdvance()
{
    auto time = QDateTime::currentMSecsSinceEpoch();

    if(m_StopAdvanse) return;

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=]()
    {
        QObject::disconnect(*conn);
        auto dt = QDateTime::currentMSecsSinceEpoch() - time;
        auto new_ad = calcAverage(m_AverageDraw, m_Field->getInformation()->getAge(), dt);
        m_Field->setWaitScene(false);

        if(m_AverageDraw < new_ad || m_AverageDraw > new_ad)
        {
            m_AverageDraw = new_ad;

            Q_EMIT signalAdvansedTime(m_AverageDraw);
        }
    };
    *conn = QObject::connect(this, &Scene::changed, func);
    advance();
}

QGraphicsRectItem *Scene::borderRect() const { return m_BorderRect; }
QHash<QPair<int, int>, SceneItem*>* Scene::itemsList() const { return const_cast<QHash<QPair<int, int>, SceneItem*>*>(&m_ItemsList); }
QSize Scene::size() const { return m_Size; }
QColor Scene::getBackgroundColor() const { return m_BackgroundColor; }
SceneView *Scene::getView() const { return m_View; }
Field *Scene::getField() const { return m_Field; }
void Scene::StopAdvanse() { m_StopAdvanse = true; }
