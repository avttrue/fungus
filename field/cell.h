#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>

class SceneObject;
class CellRule;
class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    Cell(Field *parent, CellRule* rule = nullptr);
    SceneObject *getSceneObject() const;
    void setSceneObject(SceneObject *value);
    QPoint getIndex() const;
    void setIndex(const QPoint &value);
    CellRule *getRule() const;
    void setRule(CellRule *value);
    CellInformation* getInformation();
    void clear();

private:
    Field* m_Field;
    CellRule* m_Rule;
    QPoint m_Index;
    SceneObject* m_SceneObject;
    CellInformation* m_Information;


Q_SIGNALS:

};

#endif // CELL_H
