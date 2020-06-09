#include "field.h"
#include "cellrule.h"
#include "helper.h"

#include <QDebug>

CellRule::CellRule(Field *parent)
    : QObject(parent),
      m_ColorAlive(Qt::black),
      m_CurseTime(0)
{
    setObjectName(tr("Conway's LIFE game"));

    m_Activity.append({static_cast<int>(Kernel::CellActivityType::Birth),
                       static_cast<int>(Kernel::CellActivityTarget::Near), "=", 3});
    m_Activity.append({static_cast<int>(Kernel::CellActivityType::Death),
                       static_cast<int>(Kernel::CellActivityTarget::Near), "<", 2});
    m_Activity.append({static_cast<int>(Kernel::CellActivityType::Death),
                       static_cast<int>(Kernel::CellActivityTarget::Near), ">", 3});

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellRule" << objectName() <<"destroyed"; });
    qDebug() << "CellRule" << objectName() << "created";
}

void CellRule::setColorAlive(QColor value)
{
    if (m_ColorAlive == value) return;

    m_ColorAlive = value;
    Q_EMIT signalChanged();
}

void CellRule::setActivity(CellActivity value)
{
    if (m_Activity == value) return;

    m_Activity = value;
    Q_EMIT signalChanged();
}

void CellRule::setCurseTime(int value)
{
    if (m_CurseTime == value) return;

    m_CurseTime = value;
    Q_EMIT signalChanged();
}

QString CellRule::toHtmlTable()
{
    QString content;
    auto map = getPropertiesList(this);
    content.append(QString("<tr><td class = 'TDCAPTION' colspan='2'>&#160;%1&#160;</td></tr>").
                   arg(tr("Rules \"%1\"").arg(objectName()).
                       toHtmlEscaped().
                       replace(" ", "&#160;")));

    for(auto key: map.keys())
    {
        QVariant value = property(key.toStdString().c_str());
        if(key == "XenoReaction")
        {
            value = getNameKernelEnum("CellXenoReaction", value.toInt());
            content.append(QString("<tr><td class = 'TDTEXT'>&#160;%1</td>"
                                   "<td class = 'TDTEXT'>&#160;%2</td></tr>").
                           arg(key, value.toString()));
        }
        else if(key == "ColorAlive")
        {
            content.append(QString("<tr><td class = 'TDTEXT'>&#160;%1</td>"
                                   "<td class = 'TDTEXT' bgcolor = '%2'> &#160; </td></tr>").
                           arg(key, value.toString()));
        }
        else if(key == "Activity")
        {
            auto activity = value.value<CellActivity>();
            content.append(QString("<tr><td class = 'TDTEXT' colspan='2' bgcolor = '%1'>"
                                   "&#160;%2</td></tr>").arg(CAPTION_ACTIVITY_COLOR, key));

            for(auto v: activity)
                content.append(QString("<tr><td class = 'TDTEXT' colspan='2'>&#160;%1&#160;</td></tr>").
                               arg(CellActivityElementToString(v).toHtmlEscaped().replace(" ", "&#160;")));

            // end Activity section
            content.append(QString("<tr><td class = 'TDCAPTION' colspan='2' bgcolor = '%1'>"
                                   "&#8212; &#8212; &#8212;</td></tr>").arg(CAPTION_ACTIVITY_COLOR));
        }
        else
        {
            content.append(QString("<tr><td class = 'TDTEXT'>&#160;%1</td>"
                                   "<td class = 'TDTEXT'>&#160;%2</td></tr>").
                           arg(key, value.toString()));
        }
    }
    return content;
}

QColor CellRule::getColorAlive() const { return m_ColorAlive; }
CellActivity CellRule::getActivity() const { return m_Activity; }
int CellRule::getCurseTime() const { return m_CurseTime; }
