#include "dialoginfocontent.h"
#include "properties.h"
#include "helper.h"
#include "controls.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QWindowStateChangeEvent>
#include <QDesktopServices>

DialogInfoContent::DialogInfoContent(const QString& title, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(title);
    setWindowIcon(QIcon(":/resources/img/info.svg"));
    setModal(false);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(1);
    vblForm->setSpacing(1);
    setLayout(vblForm);

    m_Content = new QTextBrowser(this);

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    toolBar->addWidget(new WidgetSpacer());

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Close"));
    actionCancel->setAutoRepeat(false);
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(m_Content);
    vblForm->addWidget(toolBar);

    setOpenLinks(false);

    installEventFilter(this);
    resize(config->InfoWindowWidth(), config->InfoWindowHeight());
    qDebug() << "DialogInfoContent" << windowTitle() << "created";
    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << "DialogInfoContent" << windowTitle() << "destroyed"; });
}

bool DialogInfoContent::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowStateChange:
    {
        if(windowState() == Qt::WindowMinimized ||
                windowState() == Qt::WindowMaximized)
        {
            setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
            return true;
        }
        return false;
    }
    case QEvent::Hide:
    case QEvent::Close:
    {
        if(object != this || isMinimized() || isMaximized()) return false;

        config->setInfoWindowWidth(width());
        config->setInfoWindowHeight(height());
        return true;
    }
    default: { return false; }
    }
}

void DialogInfoContent::setHtmlContent(const QString& content)
{
    m_Content->setHtml(content);
}

void DialogInfoContent::setMarkdownSource(const QString &source)
{
    m_Content->setSource(QUrl(source), QTextDocument::MarkdownResource);
}

void DialogInfoContent::setOpenLinks(bool on)
{
    m_Content->setOpenLinks(on);
    if(on)
    {
        QObject::disconnect(m_Content, &QTextBrowser::anchorClicked, nullptr, nullptr);
    }
    else
    {
        QObject::connect(m_Content, &QTextBrowser::anchorClicked, [=](const QUrl &link)
        {
            if(link.isEmpty() || !link.isValid()) return;
            if (!QDesktopServices::openUrl(link))
                qCritical() << __func__ << ": error at QDesktopServices::openUrl" << link.toString();
        });
    }
}
