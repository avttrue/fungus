#ifndef DIALOGINFOCONTENT_H
#define DIALOGINFOCONTENT_H

#include <QDialog>

const QString EXTERN_URL = "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}\\b([-a-zA-Z0-9()@:%_\\+.~#?&//=]*)";

class QTextBrowser;

class DialogInfoContent : public QDialog
{
    Q_OBJECT
public:
    DialogInfoContent(const QString &title, QWidget *parent = nullptr);
    void setHtmlContent(const QString& content);
    void setMarkdownSource(const QString& source);
    void setMarkdownContent(const QString& content);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTextBrowser* m_Content;
};

#endif // DIALOGINFOCONTENT_H
