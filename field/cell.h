#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>

class SceneObject;
class CellInformation;
class Field;

class Cell : public QObject
{
    Q_OBJECT

public:
    explicit Cell(QObject *parent = nullptr);
    SceneObject *getSceneObject() const;
    void setSceneObject(SceneObject *value);
    QPoint getIndex();
    void setIndex(const QPoint &value);
    CellInformation* getInformation();
    void clear();
    Field *getField() const;
    void setField(Field *value);

private:
    Field* m_Field;
    QPoint m_Index;
    SceneObject* m_SceneObject;
    CellInformation* m_Information;


Q_SIGNALS:

};

#endif // CELL_H
