#ifndef CELLRULE_H
#define CELLRULE_H

#include <QObject>

class Field;

class CellRule : public QObject
{
    Q_OBJECT
public:
    CellRule(Field *parent);


};

#endif // CELLRULE_H
