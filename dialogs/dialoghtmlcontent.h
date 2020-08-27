#ifndef DIALOGHTMLCONTENT_H
#define DIALOGHTMLCONTENT_H

#include <QDialog>

class QTextBrowser;

class DialogHtmlContent : public QDialog
{
    Q_OBJECT
public:
    DialogHtmlContent(const QString &title, QWidget *parent = nullptr);
    void setContent(const QString& title, const QString& content);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTextBrowser* m_Content;
};

#endif // DIALOGHTMLCONTENT_H
