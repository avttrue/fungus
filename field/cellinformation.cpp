#include "cellinformation.h"
#include "cell.h"

#include <QDebug>
#include <QMap>
#include <QMetaProperty>

CellInformation::CellInformation(Cell *parent)
    : QObject(parent),
      m_Age(0),
      m_Generation(0),
      m_State(Kernel::CellState::Dead)
{
    //QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "CellInformation" <<parent->objectName() <<"destroyed"; });
}

void CellInformation::setState(Kernel::CellState value)
{
    if (m_State == value) return;

    m_State = value;
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

Kernel::CellState CellInformation::getState() const { return m_State; }
int CellInformation::getAge() const { return m_Age; }
int CellInformation::getGeneration() const { return m_Generation; }
