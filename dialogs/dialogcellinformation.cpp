#include "dialogcellinformation.h"
#include "properties.h"

#include <QDebug>
#include <QIcon>
#include <QScrollArea>
#include <QToolBar>
#include <QVBoxLayout>
#include <controls.h>

DialogCellInformation::DialogCellInformation(QWidget *parent,
                                             const QString &icon,
                                             const QString &caption)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(caption);
    setWindowIcon(QIcon(icon));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(0);
    vblForm->setSpacing(0);
    setLayout(vblForm);

    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    toolBar->addWidget(new WidgetSpacer());

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), "Cancel");
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    resize(WINDOW_SIZE);

    // TODO: запретить сворачивание

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogCellInformation" << caption << "destroyed"; });
    qDebug() << "DialogCellInformation" << caption << "created";
}
