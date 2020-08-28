#ifndef DIALOGINFOCONTENT_H
#define DIALOGINFOCONTENT_H

#include <QDialog>

const QString MAINDIV_MARGIN = "10";

class QTextBrowser;

class DialogInfoContent : public QDialog
{
    Q_OBJECT
public:
    DialogInfoContent(const QString &title, QWidget *parent = nullptr);
    void setHtmlContent(const QString& content);
    void setMarkdownSource(const QString& source);
    void setOpenLinks(bool on);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTextBrowser* m_Content;
};

#endif // DIALOGINFOCONTENT_H
