#ifndef DIALOGINFOCONTENT_H
#define DIALOGINFOCONTENT_H

#include <QDialog>

class QTextBrowser;

class DialogInfoContent : public QDialog
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
};

#endif // DIALOGINFOCONTENT_H
