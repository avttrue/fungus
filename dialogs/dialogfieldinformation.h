#ifndef DIALOGFIELDINFORMATION_H
#define DIALOGFIELDINFORMATION_H

#include "dialogbody.h"

class QGridLayout;
class Field;

class DialogFieldInformation : public DialogBody
{
    Q_OBJECT
public:
    DialogFieldInformation(QWidget *parent, const QString &title, Field* field);
    Field *getField() const;

protected:
    void loadInformation();

private:
    Field* m_Field;
    QGridLayout* glContent;

};

#endif // DIALOGFIELDINFORMATION_H
