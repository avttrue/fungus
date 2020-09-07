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

DialogInfoContent::DialogInfoContent(QWidget *parent, const QString& title)
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
    m_Content->setOpenLinks(false);
    QObject::connect(m_Content, &QTextBrowser::forwardAvailable, [=](bool value)
    { m_ActionForward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::backwardAvailable, [=](bool value)
    { m_ActionBackward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::anchorClicked, [=](const QUrl &link)
    {
        if(!link.isValid()) return;
        if(!link.toString().contains(QRegExp(EXTERN_URL_RG)))
        {
            auto res_type = m_Content->sourceType();
            m_Content->setSource(link, res_type);
            return;
        }
        if(!QDesktopServices::openUrl(link))
            qCritical() << __func__ << ": error at QDesktopServices::openUrl" << link.toString();
    });

    auto toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));

    m_ActionBackward = new QAction(QIcon(":/resources/img/left_arrow.svg"), tr("Backward"));
    QObject::connect(m_ActionBackward, &QAction::triggered, [=](){ m_Content->backward(); });
    m_ActionBackward->setAutoRepeat(false);
    m_ActionBackward->setEnabled(false);
    toolBar->addAction(m_ActionBackward);


    m_ActionForward = new QAction(QIcon(":/resources/img/right_arrow.svg"), tr("Forward"));
    QObject::connect(m_ActionForward, &QAction::triggered, [=](){ m_Content->forward(); });
    m_ActionForward->setAutoRepeat(false);
    m_ActionForward->setEnabled(false);
    toolBar->addAction(m_ActionForward);


    auto m_ActionHome = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Main page"));
    QObject::connect(m_ActionHome, &QAction::triggered, [=](){ m_Content->home(); });
    m_ActionHome->setAutoRepeat(false);
    toolBar->addAction(m_ActionHome);

    toolBar->addWidget(new WidgetSpacer());

    auto actionCancel = new QAction(QIcon(":/resources/img/no.svg"), tr("Close"));
    QObject::connect(actionCancel, &QAction::triggered, [=](){ close(); });
    actionCancel->setAutoRepeat(false);
    toolBar->addAction(actionCancel);

    vblForm->addWidget(m_Content);
    vblForm->addWidget(toolBar);

    installEventFilter(this);
    resize(config->InfoWindowWidth(), config->InfoWindowHeight());

    QObject::connect(this, &QObject::destroyed, [=]()
    { qDebug() << "DialogInfoContent" << windowTitle() << "destroyed"; });
    qDebug() << "DialogInfoContent" << windowTitle() << "created";
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

void DialogInfoContent::setMarkdownContent(const QString &content)
{
    m_Content->document()->setMarkdown(content, QTextDocument::MarkdownDialectCommonMark);
}
