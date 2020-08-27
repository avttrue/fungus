#ifndef DIALOGHTMLCONTENT_H
#define DIALOGHTMLCONTENT_H

#include <QDialog>

const QString MAINDIV_MARGIN = "10";

class QTextBrowser;

class DialogHtmlContent : public QDialog
{
    Q_OBJECT
public:
    DialogHtmlContent(const QString &title, QWidget *parent = nullptr);
    void setContent(const QString& title, const QString& content);
    void setOpenLinks(bool on = false);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTextBrowser* m_Content;
};

#endif // DIALOGHTMLCONTENT_H
