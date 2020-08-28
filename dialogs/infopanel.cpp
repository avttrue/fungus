#include "infopanel.h"
#include "kernel/kernel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

InfoPanel::InfoPanel(QWidget *parent, const QString &caption, const QVariant &value)
    : QFrame(parent)
{
    setLineWidth(1);

    auto hbox = new QHBoxLayout();
    hbox->setSpacing(1);
    setLayout(hbox);

    auto labelCaption = new QLabel(caption);
    labelCaption->setStyleSheet(DCI_LABEL_STYLE);

    if(value.isValid())
    {
        hbox->setMargin(2);
        setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

        hbox->setAlignment(Qt::AlignLeft);
        hbox->addWidget(labelCaption, 1);

        m_LabelValue = new QLabel();
        m_LabelValue->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        m_LabelValue->setLineWidth(1);
        m_LabelValue->setStyleSheet(DCI_LABEL_STYLE);
        setValue(value);
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

void InfoPanel::setValue(const QVariant &value)
{
    if(value.type() == QVariant::Bool)
        value.toBool() ? m_LabelValue->setText("[+]") : m_LabelValue->setText("[ ]");
    else if(value.type() == QVariant::Double)
        m_LabelValue->setText(QString::number(value.toReal(), 'f', REAL_VALUE_PRECISION));
    else
        m_LabelValue->setText(value.toString());
}
