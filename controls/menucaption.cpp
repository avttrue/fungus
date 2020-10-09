#include "menucaption.h"

#include <QLabel>

MenuCaption::MenuCaption(QObject *parent, const QString &caption)
    : QWidgetAction(parent)
{
    auto labelCaption = new QLabel(QString("<center><h3>%1</h3></center>").arg(caption));
    labelCaption->setStyleSheet(MENU_CAPTION_STYLE);
    labelCaption->setAlignment(Qt::AlignHCenter);
    setDefaultWidget(labelCaption);
}
