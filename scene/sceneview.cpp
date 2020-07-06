#include "sceneview.h"
#include "scene.h"
#include "properties.h"
#include "sceneItem.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogcellinformation.h"
#include "dialogs/dialogvalueslist.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellinformation.h"
#include "kernel/kernel.h"

#include <QtMath>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

SceneView::SceneView(QWidget *parent)
    :QGraphicsView(parent),
      m_Scene(nullptr)
{
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | DontSavePainterState);
    SetUpdateMode();
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::NoFocus);
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_Zoomer = new GraphicsViewZoomer(this);
    m_Zoomer->SetZoomFactorBase(config->SceneScaleStep());
    m_Zoomer->setModifiers(config->SceneZoomModifier());

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "SceneView destroyed"; });
    qDebug() << "SceneView created";
}

Scene *SceneView::addScene(Field *filed)
{
    if(m_Scene) m_Scene->deleteLater();

    m_Scene = new Scene(this, filed);
    setScene(m_Scene);
    m_Scene->installEventFilter(this);

    return m_Scene;
}

bool SceneView::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_Scene)
    {
        if(event->type() != QEvent::GraphicsSceneMousePress &&
                event->type() != QEvent::GraphicsSceneMouseRelease)
        { return false; }

        auto mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);

        if(mouseSceneEvent->scenePos().x() < 0 ||
                mouseSceneEvent->scenePos().x() >= m_Scene->size().width() ||
                mouseSceneEvent->scenePos().y() < 0 ||
                mouseSceneEvent->scenePos().y() >= m_Scene->size().height())
        { return false; }

        else if(event->type() == QEvent::GraphicsSceneMousePress)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qCritical() << __func__ << ": GraphicsSceneMousePress: target item not detected";
                return true;
            }

            //qDebug() << "GraphicsSceneMousePress" << mouseSceneEvent->scenePos();
        }

        else if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qCritical() << __func__ << ": GraphicsSceneMouseRelease: target item not detected";
                return true;
            }

            auto o = static_cast<SceneItem*>(item);
            auto field = m_Scene->getField();
            field->setRunning(false);
            auto x = qFloor(mouseSceneEvent->scenePos().x() / config->SceneCellSize());
            auto y = qFloor(mouseSceneEvent->scenePos().y() / config->SceneCellSize());
            auto c = field->cells()->at(x).at(y);

            // вывод сообщения об отсутствии SceneObject
            if(mouseSceneEvent->modifiers() == Qt::NoModifier &&
                    mouseSceneEvent->button() == Qt::LeftButton && !o)
            {
                QMessageBox::information(this, tr("Information"), tr("Scene object not created yet."));
                return true;
            }

            // создать SceneObject и отредактировать Cell
            if(mouseSceneEvent->modifiers() == config->SceneObjectModifier() &&
                    mouseSceneEvent->button() == Qt::LeftButton)
            {
                auto cellinfo = c->getInformation();
                auto statelist = listKernelEnum("CellState");

                const QVector<QString> keys =
                { tr("00#_Cell properties"),
                  tr("01#_Cell state"),
                  tr("02#_Cell age"),
                  tr("03#_Cell generation") };
                QMap<QString, DialogValue> map =
                { {keys.at(0), {}},
                  {keys.at(1), {QVariant::StringList,
                                getNameKernelEnum("CellState", static_cast<int>(cellinfo->getState())), 0,
                                statelist, DialogValueMode::OneFromList}},
                  {keys.at(2), {QVariant::Int, cellinfo->getAge(), 0, 0}},
                  {keys.at(3), {QVariant::Int, cellinfo->getGeneration(), 0, 0}} };

                auto dvl = new DialogValuesList(this, ":/resources/img/point.svg",
                                                tr("Edit cell %1").arg(c->objectName()), &map);
                if(!dvl->exec()) return false;

                cellinfo->setState(static_cast<Kernel::CellState>(statelist.indexOf(map.value(keys.at(1)).value.toString())));
                cellinfo->setAge(map.value(keys.at(2)).value.toInt());
                cellinfo->setGeneration(map.value(keys.at(3)).value.toInt());

                field->setRuleOn(false);
                field->setRunningAlways(false);
                field->setRunning(true);
                field->calculate();

                return true;
            }

            //свойства Cell
                    if(mouseSceneEvent->modifiers() == config->SceneObjectModifier() &&
                       mouseSceneEvent->button() == Qt::RightButton && o)
            {
                showCellInformationDialog(c);
                m_Scene->setFocusItem(o);
                //findObjectBySell(c);
                return true;
            }

            //qDebug() << "GraphicsSceneMouseRelease" << mouseSceneEvent->scenePos();
        }
    }
    return false;
}

void SceneView::showCellInformationDialog(Cell *cell)
{
    if(DialogCellInformation::FindPreviousCopy(cell)) return;

    auto dci = new DialogCellInformation(this, cell);
    dci->show();

}

void SceneView::SetUpdateMode()
{
    if(config->SceneViewUpdateMode().toUpper() == VIEW_UPDATE_MODE.at(0))
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    else if(config->SceneViewUpdateMode().toUpper() == VIEW_UPDATE_MODE.at(1))
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    else if(config->SceneViewUpdateMode().toUpper() == VIEW_UPDATE_MODE.at(2))
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    else
    {
        qCritical() << "Wrong settins value 'Scene/ViewUpdateMode'" <<  config->SceneViewUpdateMode().toUpper();
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    }
}

Scene *SceneView::getScene() const { return m_Scene; }
GraphicsViewZoomer *SceneView::zoomer() const { return m_Zoomer; }

