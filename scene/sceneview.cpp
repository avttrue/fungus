#include "sceneview.h"
#include "scene.h"
#include "properties.h"
#include "sceneobject.h"
#include "field/field.h"
#include "field/cell.h"
#include "field/cellinformation.h"
#include "graphicsviewzoomer.h"
#include "dialogs/dialogcellinformation.h"

#include <QtMath>
#include <QDebug>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

SceneView::SceneView(QWidget *parent)
    :QGraphicsView(parent),
    m_Scene(nullptr)
{
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::NoFocus);
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHint(QPainter::Antialiasing, config->SceneViewAntialiasing());
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

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

void SceneView::fill()
{
    if(!m_Scene)
    {
        qCritical() << __func__ << ": Scene not created";
        return;
    }

    hide();

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    m_Scene->fill();

    show();
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
                qCritical() << __func__ << ": GraphicsSceneMouseEvent: target item not detected";
                return true;
            }

            //auto o = static_cast<SceneObject*>(item->toGraphicsObject());
            //if(!o)
            //{
            //    m_Scene->clearSelection();
            //    m_Scene->setFocusItem(nullptr);
            //}
            //return false;

            //qDebug() << "GraphicsSceneMousePress" << mouseSceneEvent->scenePos();
        }

        else if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            auto item = m_Scene->itemAt(mouseSceneEvent->scenePos(), transform());
            if(!item)
            {
                qCritical() << __func__ << ": GraphicsSceneMouseEvent: target item not detected";
                return true;
            }

            auto o = static_cast<SceneObject*>(item->toGraphicsObject());

            // вывод сообщения об отсутствии SceneObject
            if(mouseSceneEvent->modifiers() == Qt::NoModifier &&
                mouseSceneEvent->button() == Qt::LeftButton && !o)
            {
                QMessageBox::information(this, tr("Information"), tr("Scene object not created yet."));
                return true;
            }

            // информация о ячейке
            if(mouseSceneEvent->modifiers() == Qt::NoModifier &&
                mouseSceneEvent->button() == Qt::RightButton && o)
            {
                showCellInformationDialog(o->getCell());
                m_Scene->setFocusItem(o);
                return true;
            }

            // создать SceneObject
            if(mouseSceneEvent->modifiers() == config->SceneObjectModifier() &&
                mouseSceneEvent->button() == Qt::LeftButton && !o)
            {
                auto x = qFloor(mouseSceneEvent->scenePos().x() / config->SceneObjectSize());
                auto y = qFloor(mouseSceneEvent->scenePos().y() / config->SceneObjectSize());
                auto added = m_Scene->addObject(x, y);
                added->getCell()->clear();
                m_Scene->setFocusItem(added);
                qDebug() << added->objectName() << "created manually";
                return true;
            }
            // TODO: Вынести из SceneView редактирование Cell
            // обнулить ячейку и сделать живой
            if(mouseSceneEvent->modifiers() == config->SceneObjectModifier() &&
                mouseSceneEvent->button() == Qt::LeftButton && o)
            {
                auto c = o->getCell();
                c->clear();
                c->getInformation()->setState(Kernel::CellState::Alive);
                o->update();
                m_Scene->setFocusItem(o);

                qDebug() << "Cell" << c->objectName() << "cleared and set alive manually";
                return true;
            }
            // обнулить ячейку
            if(mouseSceneEvent->modifiers() == config->SceneObjectModifier() &&
                mouseSceneEvent->button() == Qt::RightButton && o)
            {
                auto c = o->getCell();
                c->clear();
                o->update();
                m_Scene->setFocusItem(o);

                qDebug() << "Cell" << c->objectName() << "cleared and set dead manually";
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

Scene *SceneView::getScene() const { return m_Scene; }
GraphicsViewZoomer *SceneView::zoomer() const { return m_Zoomer; }

