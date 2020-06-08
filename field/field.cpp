#include "field.h"
#include "cell.h"
#include "cellinformation.h"
#include "cellrule.h"

#include <QDebug>

Field::Field(QObject *parent,
             int width,
             int height)
    : QObject(parent),
    m_Width(width),
    m_Height(height)
{
    setObjectName(QString("FIELD[%1X%2]").arg(QString::number(width), QString::number(height)));

    m_Cells = QVector(m_Width, QVector<Cell*>(m_Height, nullptr));

    // Default rule
    m_CellRules.append(new CellRule(this));
    qDebug() << objectName() << ": rules count" << m_CellRules.count();

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

Cell *Field::addCell(int x, int y)
{
    auto c = new Cell(this);
    c->setIndex({x, y});
    c->setObjectName(QString("[%1X%2]").arg(QString::number(x), QString::number(y)));
    c->setRule(m_CellRules.at(0)); // default rule
    m_Cells[x][y] = c;

    Q_EMIT signalCellAdded(c);
    return c;
}

Cell *Field::getTopCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.y() == 0) index.setY(height() - 1);
    else index.setY(index.y() - 1);

    return getCell(index);
}

Cell *Field::getRightCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    return getCell(index);
}

Cell *Field::getBottomCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    return getCell(index);
}

Cell *Field::getLeftCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    return getCell(index);
}

Cell *Field::getTopLeftCell(Cell *c) const
{
   auto index = c->getIndex();

   if(index.x() == 0) index.setX(width() - 1);
   else index.setX(index.x() - 1);

   if(index.y() == 0) index.setY(height() - 1);
   else index.setY(index.y() - 1);

   return getCell(index);
}

Cell *Field::getTopRightCell(Cell *c) const
{
  auto index = c->getIndex();

  if(index.y() == 0) index.setY(height() - 1);
  else index.setY(index.y() - 1);

  if(index.x() == width() - 1) index.setX(0);
  else index.setX(index.x() + 1);

  return getCell(index);
}

Cell *Field::getBottomLeftCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == 0) index.setX(width() - 1);
    else index.setX(index.x() - 1);

    return getCell(index);
}

Cell *Field::getBottomRightCell(Cell *c) const
{
    auto index = c->getIndex();

    if(index.y() == height() - 1) index.setY(0);
    else index.setY(index.y() + 1);

    if(index.x() == width() - 1) index.setX(0);
    else index.setX(index.x() + 1);

    return getCell(index);
}

QVector<Cell*> Field::getAliveCells(Cell *c) const
{
    QVector<Cell*> result;

    auto cell = getTopCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getTopRightCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getRightCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getBottomRightCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getBottomCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getBottomLeftCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getLeftCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    cell = getTopLeftCell(c);
    if(cell->getInformation()->getState() == Kernel::CellState::Alive) result.append(cell);

    return result;
}


Cell *Field::getCell(QPoint index) const { return m_Cells[index.x()][index.y()]; }
QVector<QVector<Cell *>> *Field::cells() const { return const_cast<QVector<QVector<Cell*>>*>(&m_Cells); }
int Field::height() const { return m_Height; }
int Field::width() const { return m_Width; }
