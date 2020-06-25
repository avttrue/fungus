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
    m_Size = QSize(m_Field->width() * config->SceneCellSize(), m_Field->height()) * config->SceneCellSize();
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

    addSceneItem();

    QObject::connect(m_Field, &Field::signalCalculated, this, &Scene::slotAdvance, Qt::DirectConnection);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void Scene::addSceneItem()
{
    m_SceneItem = new SceneItem(this);
    m_SceneItem->setPos(m_SceneItem->mapToParent(0, 0));
    addItem(m_SceneItem);
}

void Scene::setBackgroundColor(const QColor &value)
{
    m_BackgroundColor = value;
    setBackgroundBrush(m_BackgroundColor);
}

void Scene::slotAdvance(const QPixmap &pixmap)
{
    auto time = QDateTime::currentMSecsSinceEpoch();

    if(m_StopAdvanse) return;

    m_SceneItem->setPixmap(pixmap);

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
QSize Scene::size() const { return m_Size; }
QColor Scene::getBackgroundColor() const { return m_BackgroundColor; }
SceneView *Scene::getView() const { return m_View; }
Field *Scene::getField() const { return m_Field; }
void Scene::StopAdvanse() { m_StopAdvanse = true; }
SceneItem *Scene::getSceneItem() const { return m_SceneItem; }
