#ifndef DIALOGBODY_H
#define DIALOGBODY_H

#include <QDialog>
#include <QLabel>

const QString DB_CAPTION_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                 "stop: 0 Silver, stop: 0.5 PowderBlue, "
                                 "stop: 0.5 PowderBlue, stop: 1 silver); "
                                 "border: 1px solid Silver; "
                                 "padding: 0px 1px 0px 1px; border-radius: 5px; "
                                 "color: black; font-family: monospace; font: bold; ";

const QString DB_ICON_STYLE = "background: qradialgradient(cx:0.5, cy:0.5, radius: 1, "
                              "fx:0.5, fy:0.5, stop:0 white, stop:1 silver); "
                              "border-radius: 5px; "
                              "padding: 1px; "
                              "margin: 1px; ";

const QString DB_WINDOW_STYLE = "QDialog { border-style: ridge; "
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

protected:
    bool eventFilter(QObject *object, QEvent *event);

Q_SIGNALS:
    void signalSizeChanged(QSize value);

};


#endif // DIALOGBODY_H
