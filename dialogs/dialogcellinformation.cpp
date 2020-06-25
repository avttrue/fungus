#include "dialogcellinformation.h"
#include "properties.h"
#include "controls.h"
#include "helper.h"
#include "field/cell.h"
#include "field/field.h"
#include "field/cellrule.h"
#include "field/cellinformation.h"
#include "field/fieldservice.h"
#include "scene/scene.h"
#include "scene/sceneItem.h"
#include "scene/sceneview.h"

#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QTextEdit>
#include <QFileDialog>
#include <QCheckBox>

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
    setWindowIcon(QIcon(":/resources/img/point.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(0);
    vblForm->setSpacing(0);
    setLayout(vblForm);

    m_TEContent = new QTextEdit(this);
    m_TEContent->setLineWrapMode(QTextEdit::NoWrap);
    m_TEContent->setReadOnly(true);
    m_TEContent->setUndoRedoEnabled(false);
    m_TEContent->document()->setDocumentMargin(0);

    m_CBShowRule = new QCheckBox(tr("Show rules"), this);
    m_CBShowRule->setCheckState(Qt::Unchecked);
    QObject::connect(m_CBShowRule, &QCheckBox::stateChanged, [=]() { loadInformation(); });

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto actionShowPoint = new QAction(QIcon(":/resources/img/check.svg"), "Show point");
    actionShowPoint->setAutoRepeat(false);
    QObject::connect(actionShowPoint, &QAction::triggered, this, &DialogCellInformation::slotShowPoint);
    toolBar->addAction(actionShowPoint);

    auto actionSave = new QAction(QIcon(":/resources/img/save.svg"), "Save");
    actionSave->setAutoRepeat(false);
    QObject::connect(actionSave, &QAction::triggered, this, &DialogCellInformation::slotSaveContent);
    toolBar->addAction(actionSave);

    toolBar->addSeparator();
    toolBar->addWidget(m_CBShowRule);

    toolBar->addWidget(new WidgetSpacer(this));

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), "Cancel");
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);    

    vblForm->addWidget(m_TEContent);
    vblForm->addWidget(toolBar);

    loadInformation();

    QObject::connect(cell->getInformation(), &CellInformation::signalStateChanged,
                     this, &DialogCellInformation::loadInformation, Qt::QueuedConnection);
    QObject::connect(cell->getInformation(), &CellInformation::signalAgeChanged,
                     this, &DialogCellInformation::loadInformation, Qt::QueuedConnection);
    QObject::connect(cell->getInformation(), &CellInformation::signalGenerationChanged,
                     this, &DialogCellInformation::loadInformation, Qt::QueuedConnection);
    QObject::connect(cell, &QObject::destroyed, this, &QDialog::close); // закрывать при уничтожении cell

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
    auto sb_pos = m_TEContent->verticalScrollBar()->value();
    auto map = getPropertiesList(m_Cell->getInformation());
    QString content;    

    // Cell information
    for(auto key: map.keys())
    {
        QVariant value = m_Cell->getInformation()->property(key.toStdString().c_str());
        if(key == "State")
        {
            value = getNameKernelEnum("CellState", value.toInt());
        }
        content.append(QString("<tr><td class = 'TDTEXT'>&#160;%1</td>"
                               "<td class = 'TDTEXT'>&#160;%2</td></tr>").
                       arg(key, value.toString()));
    }

    // Cell rules
    if(m_CBShowRule->isChecked())
        content.append(m_Cell->getField()->getRule()->toHtmlTable());

    QString table = getTextFromRes(":/resources/table_content.html").
                    arg(windowTitle(), content);
    QString html = getTextFromRes(":/resources/cellinformation.html").
                   arg(windowTitle(), table);

    m_TEContent->setProperty(TB_PROPERTY_CONTENT, html);
    m_TEContent->setHtml(html);
    m_TEContent->verticalScrollBar()->setValue(sb_pos);
}

void DialogCellInformation::slotSaveContent()
{
    auto deffilename = config->LastDir() + QDir::separator() + QString("%1.html").arg(windowTitle());
    auto filename = QFileDialog::getSaveFileName(this, "Save cell report", deffilename, "HTML files (*.html)");

    if(filename.isNull()) return;
    config->setLastDir(QFileInfo(filename).dir().path());
    if(!filename.endsWith(".html", Qt::CaseInsensitive)) filename.append(".html");

    QString text = m_TEContent->property(TB_PROPERTY_CONTENT).toString();

    if(textToFile(text, filename)) return;

    qCritical() << __func__ << "Error at file saving:" << filename;
}

void DialogCellInformation::slotShowPoint()
{
//    auto o = m_Cell->getSceneItem();
//    o->getScene()->getView()->findObjectBySell(m_Cell);
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

Cell *DialogCellInformation::getCell() const { return m_Cell; }

