#ifndef TEXTLOG_H
#define TEXTLOG_H

#include <QPlainTextEdit>

const QString LINE_SEPARATOR = "------------------------------------------";

class TextLog : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextLog(QWidget *parent);
    void addTextSeparator();

private:
    QAction* m_ActionClearText;

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // TEXTLOG_H
