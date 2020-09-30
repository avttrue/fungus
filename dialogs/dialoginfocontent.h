#ifndef DIALOGINFOCONTENT_H
#define DIALOGINFOCONTENT_H

#include "dialogbody.h"

class QTextBrowser;

class DialogInfoContent : public DialogBody
{
    Q_OBJECT
public:
    DialogInfoContent(QWidget *parent, const QString &title);
    void setHtmlContent(const QString& content);
    void setMarkdownSource(const QString& source);
    void setMarkdownContent(const QString& content);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTextBrowser* m_Content;
    QAction* m_ActionBackward;
    QAction* m_ActionForward;

private Q_SLOTS:
    void slotAnchorClicked(const QUrl &link);
};

#endif // DIALOGINFOCONTENT_H
