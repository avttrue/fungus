#ifndef DIALOGFIELDINFORMATION_H
#define DIALOGFIELDINFORMATION_H

#include <QDialog>

class QGridLayout;
class Field;

class DialogFieldInformation : public QDialog
{
    Q_OBJECT
public:
    DialogFieldInformation(QWidget *parent, Field* field);
    static bool FindPreviousCopy(Field* field); // найти предыдущую копию окна, если найдена, то возвращается TRUE и делает его активным
    Field *getField() const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Field* m_Field;
    QGridLayout* glContent;
};

#endif // DIALOGFIELDINFORMATION_H
