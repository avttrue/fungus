#ifndef DIALOGFIELDINFORMATION_H
#define DIALOGFIELDINFORMATION_H

#include <QDialog>

class QGridLayout;
class Field;

class DialogFieldInformation : public QDialog
{
    Q_OBJECT
public:
    DialogFieldInformation(QWidget *parent, const QString &title, Field* field);
    Field *getField() const;

protected:
    bool eventFilter(QObject *object, QEvent *event);    
    void loadInformation();

private:
    Field* m_Field;
    QGridLayout* glContent;

};

#endif // DIALOGFIELDINFORMATION_H
