#include "dialoginfopanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

DialogInfoPanel::DialogInfoPanel(QWidget *parent, const QString &caption, const QString &value)
    : QFrame(parent)
{
    setLineWidth(1);

    auto hbox = new QHBoxLayout();
    hbox->setSpacing(1);
    setLayout(hbox);

    auto labelCaption = new QLabel(caption, this);
    labelCaption->setStyleSheet(DCI_LABEL_STYLE);

    if(!value.isEmpty())
    {
        hbox->setMargin(2);
        setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

        hbox->setAlignment(Qt::AlignLeft);
        hbox->addWidget(labelCaption, 1);

        m_LabelValue = new QLabel(value, this);
        m_LabelValue->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        m_LabelValue->setLineWidth(1);
        m_LabelValue->setStyleSheet(DCI_LABEL_STYLE);
        hbox->addWidget(m_LabelValue, 1);
    }
    else
    {
        hbox->setMargin(3);
        setFrameStyle(QFrame::Panel | QFrame::Raised);

        hbox->setAlignment(Qt::AlignAbsolute);
        hbox->addWidget(labelCaption, 0);
    }

    setProperty(DCI_INFOPANEL_KEY_PROPERTY, caption);
}

void DialogInfoPanel::setValue(const QVariant &value) { m_LabelValue->setText(value.toString()); }
