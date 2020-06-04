#include "dialogcellinformation.h"
#include "properties.h"
#include "controls.h"
#include "helper.h"
#include "field/cell.h"
#include "field/cellinformation.h"

#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWindowStateChangeEvent>
#include <QTextBrowser>
#include <QFileDialog>

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

    textBrowser = new QTextBrowser(this);
    textBrowser->setOpenLinks(false);
    textBrowser->setUndoRedoEnabled(false);

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    auto actionSave = new QAction(QIcon(":/resources/img/save.svg"), "Save");
    actionSave->setAutoRepeat(false);
    QObject::connect(actionSave, &QAction::triggered, this, &DialogCellInformation::saveContent);
    toolBar->addAction(actionSave);

    toolBar->addWidget(new WidgetSpacer(this));

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), "Cancel");
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);    

    vblForm->addWidget(textBrowser);
    vblForm->addWidget(toolBar);

    loadInformation();

    QObject::connect(cell->getInformation(), &CellInformation::signalChanged, this, &DialogCellInformation::loadInformation);

    installEventFilter(this);
    resize(WINDOW_SIZE);

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogCellInformation destroyed"; });
    qDebug() << "DialogCellInformation created";
}

bool DialogCellInformation::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    switch (event->type())
    {
    case QEvent::WindowStateChange:
    {
        if(!isMinimized()) return false;

        // если всё же свернули
        setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
        return true;
    }
    default: { return false; }
    }
}

void DialogCellInformation::loadInformation()
{
    int sb_pos = textBrowser->verticalScrollBar()->value();

    QString content;

    auto map = m_Cell->getInformation()->getPropertiesList();

    for(auto key: map.keys())
    {
        if(map.value(key) != QVariant::Bool)
        {
            content.append(QString("<tr><td class = 'TDTEXT'>%1</td>"
                                   "<td class = 'TDTEXT'>%2</td></tr>").
                           arg(key, m_Cell->getInformation()->property(key.toStdString().c_str()).toString()));
        }
        else if(map.value(key) == QVariant::Bool)
        {
            auto value =  m_Cell->getInformation()->property(key.toStdString().c_str()).toBool();
            content.append(QString("<tr><td class = 'TDTEXT'>%1</td>"
                                   "<td class = 'TDTEXT'>%2</td></tr>").
                           arg(key, value ? TAG_YES : TAG_NO));
        }
    }

    QString html = getTextFromRes(":/resources/cellinformation.html").
                   arg(windowTitle(), content);

    textBrowser->setProperty(TB_PROPERTY_CONTENT, html);

    textBrowser->setHtml(html);
    textBrowser->verticalScrollBar()->setValue(sb_pos);
}

void DialogCellInformation::saveContent()
{
    auto deffilename = config->LastDir() + QDir::separator() + QString("%1.html").arg(windowTitle());
    auto filename = QFileDialog::getSaveFileName(this, "Save cell report", deffilename, "HTML files (*.html)");

    if(filename.isNull()) return;
    config->setLastDir(QFileInfo(filename).dir().path());
    if(!filename.endsWith(".html", Qt::CaseInsensitive)) filename.append(".html");

    QFile file(filename);

    QString text = textBrowser->property(TB_PROPERTY_CONTENT).toString();

    if(textToFile(text, filename)) return;

    qCritical() << __func__ << "Error at file saving:" << filename;
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

