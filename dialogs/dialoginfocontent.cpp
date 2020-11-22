#include "dialoginfocontent.h"
#include "properties.h"
#include "helpers/helper.h"
#include "helpers/widgethelper.h"
#include "controls/separators.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTextBrowser>

DialogInfoContent::DialogInfoContent(QWidget *parent, const QString& title)
    : DialogBody(parent, title, ":/resources/img/info.svg")
{    
    m_Content = new QTextBrowser(this);
    m_Content->setOpenLinks(false);
    QObject::connect(m_Content, &QTextBrowser::forwardAvailable, [=](bool value)
    { m_ActionForward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::backwardAvailable, [=](bool value)
    { m_ActionBackward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::anchorClicked, this, &DialogInfoContent::slotAnchorClicked);
    
    ToolBar()->setIconSize(QSize(config->ButtonSize(), config->ButtonSize()));
    
    m_ActionBackward = new QAction(QIcon(":/resources/img/left_arrow.svg"), tr("Backward"));
    QObject::connect(m_ActionBackward, &QAction::triggered, [=](){ m_Content->backward(); });
    m_ActionBackward->setAutoRepeat(false);
    m_ActionBackward->setEnabled(false);
    addToolBarAction(ToolBar(), m_ActionBackward, CSS_TOOLBUTTON);
    
    m_ActionForward = new QAction(QIcon(":/resources/img/right_arrow.svg"), tr("Forward"));
    QObject::connect(m_ActionForward, &QAction::triggered, [=](){ m_Content->forward(); });
    m_ActionForward->setAutoRepeat(false);
    m_ActionForward->setEnabled(false);
    addToolBarAction(ToolBar(), m_ActionForward, CSS_TOOLBUTTON);
    
    auto m_ActionHome = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Main page"));
    QObject::connect(m_ActionHome, &QAction::triggered, [=](){ m_Content->home(); });
    m_ActionHome->setAutoRepeat(false);
    addToolBarAction(ToolBar(), m_ActionHome, CSS_TOOLBUTTON);
    
    ToolBar()->addWidget(new WidgetSpacer());
    
    addDialogContent(m_Content);
    
    resize(DIC_WINDOW_SIZE);
    
    QObject::connect(this, &QObject::destroyed, [=]()
    { qDebug() << "DialogInfoContent" << windowTitle() << "destroyed"; });
    qDebug() << "DialogInfoContent" << windowTitle() << "created";
}

void DialogInfoContent::slotAnchorClicked(const QUrl &link)
{
    qDebug() << __func__;
    
    if(!link.isValid()) return;
    
    if(!link.toString().contains(QRegExp(EXTERN_URL_RG)))
    {
        auto source_page = m_Content->source().toString();
        auto target_page = link.toString();
        
        qDebug() << "Source page:" << source_page;
        qDebug() << "Target page:"  << target_page;
        
        if(source_page.endsWith(HELP_PAGE_TRIGGER)) config->setHelpPage(target_page);

        auto res_type = m_Content->sourceType();
        m_Content->setSource(link, res_type);
        return;
    }
    
    OpenUrl(link);
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
