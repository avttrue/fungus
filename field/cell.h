#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>

class SceneItem;
class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    explicit Cell(QObject *parent = nullptr);
    SceneItem *getSceneItem() const;
    void setSceneObject(SceneItem *value);
    QPoint getIndex();
    void setIndex(const QPoint &value);
    CellInformation* getInformation();
    void clear();
    Field *getField() const;
    void setField(Field *value);

private:
    Field* m_Field;
    QPoint m_Index;
    SceneItem* m_SceneObject;
    CellInformation* m_Information;


Q_SIGNALS:

};

#endif // CELL_H
