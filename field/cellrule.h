#ifndef CELLRULE_H
#define CELLRULE_H

#include "fieldservice.h"

#include <QColor>
#include <QObject>

class Field;

class CellRule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Kernel::CellXenoReaction XenoReaction READ getXenoReaction WRITE setXenoReaction NOTIFY signalChanged)
    Q_PROPERTY(QColor ColorAlive READ getColorAlive WRITE setColorAlive NOTIFY signalChanged)
    Q_PROPERTY(CellActivity Activity READ getActivity WRITE setActivity NOTIFY signalChanged)

public:
    CellRule(Field *parent);

    Kernel::CellXenoReaction getXenoReaction() const;
    void setXenoReaction(Kernel::CellXenoReaction value);
    QColor getColorAlive() const;
    void setColorAlive(QColor Color);
    CellActivity getActivity() const;
    void setActivity(CellActivity Activity);

private:
    QColor m_ColorAlive;
    CellActivity m_Activity;
    Kernel::CellXenoReaction m_XenoReaction;

Q_SIGNALS:
    void signalChanged();
};



#endif // CELLRULE_H
