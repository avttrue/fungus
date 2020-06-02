#include "field.h"
#include "cell.h"

#include <QDebug>
#include <QDateTime> //

Field::Field(QObject *parent,
             int width,
             int height)
    : QObject(parent),
    m_Width(width),
    m_Height(height)
{
    setObjectName(QString("FIELD[%1X%2]").arg(QString::number(width), QString::number(height)));

    m_Cells = QVector(m_Width, QVector<Cell*>(m_Height, nullptr));

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

Cell *Field::addCell(int x, int y)
{
    auto c = new Cell(this);
    c->setIndex({x, y});
    c->setObjectName(QString("[%1X%2]").arg(QString::number(x), QString::number(y)));
    m_Cells[x][y] = c;

    Q_EMIT signalCellAdded(c);
    return c;
}

QVector<QVector<Cell *>> *Field::cells() { return &m_Cells; }
int Field::height() const { return m_Height; }
int Field::width() const { return m_Width; }
