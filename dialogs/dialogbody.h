#ifndef DIALOGBODY_H
#define DIALOGBODY_H

#include <QDialog>
#include <QLabel>

const QString CAPTION_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                              "stop: 0 transparent, "
                              "stop: 0.5 silver, "
                              "stop: 0.5 silver, "
                              "stop: 1 transparent); "
                              "border: 1px solid silver; "
                              "padding: 0px 1px 0px 1px; border-radius: 5px; "
                              "color: black; font-family: monospace; font: bold; ";

const QString ICON_STYLE = "padding: 0px 0px 0px 0px; "
                           "margin: 1px 1px 1px 1px; ";

const QString WINDOW_STYLE = "QDialog { border-style: ridge; "
                             "border-width: 2px; "
                             "border-radius: 5px; "
                             "border-color: silver; }";

class QGridLayout;
class QToolBar;

/*!
 * \brief Caption class - заголовок окна с возможностью перетаскивания
 */
class DialogCaption : public QLabel
{
    Q_OBJECT

public:
    DialogCaption(const QString &text, QWidget* parent = nullptr);

public Q_SLOTS:
    void setText(const QString& text);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_MouseClick_X;
    int m_MouseClick_Y;
};

/////////////////////////////////////////////////////

class DialogBody : public QDialog
{
    Q_OBJECT

public:
    DialogBody(QWidget* parent,
               const QString& text,
               const QString& icon,
               bool closable = true,
               bool modal = false);

    void addDialogContent(QWidget* widget);
    QToolBar *ToolBar() const;

private:
    QGridLayout* m_ContentGridLayout;
    DialogCaption* m_Caption;
    QToolBar* m_ToolBar;
};


#endif // DIALOGBODY_H
