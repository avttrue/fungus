#include "field.h"
#include "cell.h"
#include "cellinformation.h"
#include "cellrule.h"

#include <QDateTime>
#include <QDebug>

Field::Field(QObject *parent, int width, int height)
    : QObject(parent),
      m_Width(width),
      m_Height(height)
{
    setObjectName(QString("FIELD[%1X%2]").arg(QString::number(width), QString::number(height)));

    m_Cells = QVector(m_Width, QVector<Cell*>(m_Height, nullptr));

    // Default rule
    auto rule = new CellRule(this);
    m_DefaultRule = rule->objectName();
    m_CellRules.insert(m_DefaultRule, rule);

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

Cell *Field::addCell(QPoint index) { return addCell(index.x(), index.y()); }

Cell *Field::getTopCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getTopLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getTopRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomLeftCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

Cell *Field::getBottomRightCell(Cell *c)
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    auto cell = getCell(index);
    if(!cell) cell = addCell(index);

    return cell;
}

QVector<Cell*> Field::getCellsAround(Cell *c)
{
    QVector<Cell*> result;

    result.append(getTopCell(c));
    result.append(getTopRightCell(c));
    result.append(getRightCell(c));
    result.append(getBottomRightCell(c));
    result.append(getBottomCell(c));
    result.append(getBottomLeftCell(c));
    result.append(getLeftCell(c));
    result.append(getTopLeftCell(c));

    return result;
}

void Field::fill()
{
    auto time = QDateTime::currentMSecsSinceEpoch();
    for(int h = 0; h < m_Height; h++)
    {
        for(int w = 0; w < m_Width; w++)
        {
            auto c = addCell(w, h);
            Q_EMIT signalCellAdded(c);
        }
        Q_EMIT signalProgress(h + 1);
    }
    qDebug() << "field" << objectName() << "filled in"
             << QDateTime::currentMSecsSinceEpoch() - time << "ms";
}

Cell *Field::getCell(QPoint index) { return m_Cells[index.x()][index.y()]; }
QVector<QVector<Cell *>> *Field::cells() const { return const_cast<QVector<QVector<Cell*>>*>(&m_Cells); }
int Field::height() { return m_Height; }
int Field::width() { return m_Width; }
QMap<QString, CellRule *> Field::getCellRules() const { return m_CellRules; }
