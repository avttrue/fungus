#include "helpergraphics.h"

#include <QBuffer>
#include <QIcon>
#include <QDebug>
#include <QPainter>

QString getBase64Image(const QString& path, QSize size, bool html)
{
    QIcon icon(path);
    return getBase64Image(icon.pixmap(size), html);
}

QString getBase64Image(QPixmap pixmap, bool html)
{
    QImage image(pixmap.toImage());
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG");
    QString stringBase64 = html ? "data:image/png;base64," : "";
    stringBase64.append(QString::fromLatin1(byteArray.toBase64().data()));
    return stringBase64;
}

QPixmap getPixmapFromBase64(const QString& img, bool* ok, int defsize)
{
    QPixmap pixmap;
    bool result = true;
    if(!pixmap.loadFromData(QByteArray::fromBase64(img.toLatin1())))
    {
        pixmap = QIcon(":/resources/img/error.svg").pixmap(defsize, defsize);
        result = false;
        qCritical() << __func__ << ": Error loading from Base64";
    }
    if(ok) *ok = result;
    return pixmap;
}

QColor GetContrastColor(const QColor &color)
{
    const int SATURATION_THRESHOLD = 51;
    int h = color. hslHue();
    int s = color.hslSaturation();
    int l = color.lightness();

    h = h + 180;
    if (h >= 360) h -= 360;
    if (s < SATURATION_THRESHOLD) l = l < 128 ? 255 : 0;

    return QColor::fromHsl(h, 255, l, color.alpha());
}
