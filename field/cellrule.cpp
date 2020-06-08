#include "field.h"
#include "cellrule.h"

#include <QDebug>

CellRule::CellRule(Field *parent)
    : QObject(parent),
      m_ColorAlive(Qt::black),
      m_XenoReaction(Kernel::CellXenoReaction::AsOwn)
{
    setObjectName(tr("Conway's LIFE game"));

    CellActivity activity;
    activity.append({"Birth", {"=", 3}});
    activity.append({"Death", {"<", 2}});
    activity.append({"Death", {">", 3}});

    m_Activity = activity;

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellRule" << objectName() <<"destroyed"; });
    qDebug() << "CellRule" << objectName() << "created";
}

void CellRule::setXenoReaction(Kernel::CellXenoReaction value)
{
    if (m_XenoReaction == value) return;

    m_XenoReaction = value;
    Q_EMIT signalChanged();
}

void CellRule::setColorAlive(QColor Color)
{
    if (m_ColorAlive == Color) return;

    m_ColorAlive = Color;
    Q_EMIT signalChanged();
}

void CellRule::setActivity(CellActivity Activity)
{
    if (m_Activity == Activity) return;

    m_Activity = Activity;
    Q_EMIT signalChanged();
}

Kernel::CellXenoReaction CellRule::getXenoReaction() const { return m_XenoReaction; }
QColor CellRule::getColorAlive() const { return m_ColorAlive; }
CellActivity CellRule::getActivity() const { return m_Activity; }
