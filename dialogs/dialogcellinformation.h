#ifndef DIALOGCELLINFORMATION_H
#define DIALOGCELLINFORMATION_H

#include <QDialog>

const QSize WINDOW_SIZE(500, 400);

class QGridLayout;

class DialogCellInformation : public QDialog
{
    Q_OBJECT
public:
    DialogCellInformation(QWidget *parent,
                          const QString& icon,
                          const QString& caption);

private:
    QGridLayout* glContent;
};

#endif // DIALOGCELLINFORMATION_H
