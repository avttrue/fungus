#include "dialogcellinformation.h"
#include "properties.h"
#include "controls.h"
#include "helper.h"
#include "field/cell.h"
#include "field/field.h"
#include "field/cellrule.h"
#include "field/cellinformation.h"
#include "scene/scene.h"
#include "scene/sceneItem.h"
#include "scene/sceneview.h"

#include <QDebug>
#include <QScrollBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QLabel>
#include <QApplication>

DialogCellInformation::DialogCellInformation(QWidget *parent,
                                             Cell *cell)
    : QDialog(parent),
      m_Cell(cell)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Cell %1").arg(cell->objectName()));
    setWindowIcon(QIcon(":/resources/img/eye.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(1);
    vblForm->setSpacing(1);
    setLayout(vblForm);

    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);
    glContent->setMargin(1);
    glContent->setSpacing(1);

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto actionShowPoint = new QAction(QIcon(":/resources/img/point.svg"), "Show point");
    actionShowPoint->setAutoRepeat(false);
    QObject::connect(actionShowPoint, &QAction::triggered, this, &DialogCellInformation::slotShowPoint);
    toolBar->addAction(actionShowPoint);

    toolBar->addSeparator();

    toolBar->addWidget(new WidgetSpacer(this));

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), "Cancel");
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    loadInformation();

    QObject::connect(cell->getCurInfo(), &CellInformation::signalStateChanged,
                     this, &DialogCellInformation::slotCellStateChanged, Qt::QueuedConnection);
    QObject::connect(cell->getCurInfo(), &CellInformation::signalAgeChanged,
                     this, &DialogCellInformation::slotCellAgeChanged, Qt::QueuedConnection);
    QObject::connect(cell->getCurInfo(), &CellInformation::signalGenerationChanged,
                     this, &DialogCellInformation::slotCellGenerationChanged, Qt::QueuedConnection);
    QObject::connect(cell, &QObject::destroyed, this, &QDialog::close, Qt::DirectConnection); // закрывать при уничтожении cell

    installEventFilter(this);
    resize(config->CellInfoWindowWidth(), config->CellInfoWindowHeight());
}

bool DialogCellInformation::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowStateChange:
    {
        if(!isMinimized()) return false;

        // если всё же свернули
        setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
        return true;

    }
    case QEvent::Close:
    {
        if(object != this || isMinimized() || isMaximized()) return false;

        QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
        settings.setValue("MainWindow/CellInfoWindowHeight", height());
        settings.setValue("MainWindow/CellInfoWindowWidth", width());

        return true;
    }

    default: { return false; }
    }
}

void DialogCellInformation::loadInformation()
{
    glContent->addWidget(new DialogInfoPanel(this, tr("Properties"), ""));

    auto map = getPropertiesList(m_Cell->getCurInfo());
    QString content;

    // Cell information
    for(auto key: map.keys())
    {
        QVariant value = m_Cell->getCurInfo()->property(key.toStdString().c_str());

        if(key == "State") value = getNameKernelEnum("CellState", value.toInt());
        glContent->addWidget(new DialogInfoPanel(this, key, value.toString()));
    }
}

bool DialogCellInformation::FindPreviousCopy(Cell *cell)
{    
    for (QWidget *widget: QApplication::topLevelWidgets())
    {
        auto dci = qobject_cast<DialogCellInformation*>(widget);

        if (!dci) continue;

        if(dci->getCell() == cell)
        {
            dci->showNormal();
            dci->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}

void DialogCellInformation::setValue(const QString &key, const QVariant &value)
{
    for(int i = 0; i < glContent->count(); i++)
    {
        auto item = glContent->itemAt(i)->widget();
        if(item->property(DCI_INFOPANEL_KEY_PROPERTY) == key)
        {
            auto dip = qobject_cast<DialogInfoPanel*>(item);
            dip->setValue(value.toString());
            return;
        }
    }

    qCritical() << __func__ << "property" <<  DCI_INFOPANEL_KEY_PROPERTY << "not found";
}

void DialogCellInformation::slotShowPoint() { Q_EMIT signalShowCell(m_Cell); }
Cell *DialogCellInformation::getCell() const { return m_Cell; }
void DialogCellInformation::slotCellAgeChanged(qint64 value) { setValue("Age", value); }
void DialogCellInformation::slotCellStateChanged(int value) { setValue("State", getNameKernelEnum("CellState", value)); }
void DialogCellInformation::slotCellGenerationChanged(qint64 value) { setValue("Generation", value); }

//----------------------------------------------------

DialogInfoPanel::DialogInfoPanel(QWidget *parent, const QString &caption, const QString &value)
    : QFrame(parent)
{
    setLineWidth(1);

    auto hbox = new QHBoxLayout();
    hbox->setSpacing(1);
    setLayout(hbox);

    auto labelCaption = new QLabel(caption, this);
    labelCaption->setStyleSheet(DCI_LABEL_STYLE);

    if(!value.isEmpty())
    {
        hbox->setMargin(2);
        setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

        hbox->setAlignment(Qt::AlignLeft);
        hbox->addWidget(labelCaption, 1);

        m_LabelValue = new QLabel(value, this);
        m_LabelValue->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        m_LabelValue->setLineWidth(1);
        m_LabelValue->setStyleSheet(DCI_LABEL_STYLE);
        hbox->addWidget(m_LabelValue, 1);
    }
    else
    {
        hbox->setMargin(3);
        setFrameStyle(QFrame::Panel | QFrame::Raised);

        hbox->setAlignment(Qt::AlignAbsolute);
        hbox->addWidget(labelCaption, 0);
    }

    setProperty(DCI_INFOPANEL_KEY_PROPERTY, caption);
}

void DialogInfoPanel::setValue(const QString &value) { m_LabelValue->setText(value); }
