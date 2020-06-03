#include "cellinformation.h"
#include "cell.h"

#include <QDebug>
#include <QMap>
#include <QMetaProperty>

CellInformation::CellInformation(Cell *parent)
    : QObject(parent),
    m_Age(0),
    m_Generation(0),
    m_Alive(false)
{
    //QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellInformation" <<parent->objectName() <<"destroyed"; });
}

void CellInformation::setAlive(bool value)
{
    if (m_Alive == value) return;

    m_Alive = value;
    Q_EMIT signalChanged();
}

void CellInformation::setAge(int value)
{
    if (m_Age == value) return;

    m_Age = value;
    Q_EMIT signalChanged();
}

void CellInformation::setGeneration(int value)
{
    if (m_Generation == value) return;

    m_Generation = value;
    Q_EMIT signalChanged();
}

QMap<QString, QVariant::Type> CellInformation::getPropertiesList()
{
    QMap<QString, QVariant::Type> result;

    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        result.insert(metaObject()->property(i).name(), metaObject()->property(i).type());
    }

    return result;
}

bool CellInformation::getAlive() const { return m_Alive; }
int CellInformation::getAge() const { return m_Age; }
int CellInformation::getGeneration() const { return m_Generation; }
