#include "textlog.h"

#include <QAction>
#include <QIcon>
#include <QMenu>

TextLog::TextLog(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setUndoRedoEnabled(false);
    setReadOnly(true);
    setWordWrapMode(QTextOption::NoWrap);
    setFont(QFont("monospace", -1, QFont::Bold));

    m_ActionClearText = new QAction(QIcon(":/resources/img/delete.svg"), tr("Clear all"), this);
    m_ActionClearText->setAutoRepeat(false);
    QObject::connect(m_ActionClearText, &QAction::triggered, [=](){ clear(); });
}

void TextLog::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    menu->addAction(m_ActionClearText);
    menu->exec(event->globalPos());
    delete menu;
}

void TextLog::addTextSeparator()
{
    appendHtml(QString("<p>%1</p>").arg(LINE_SEPARATOR));
}
