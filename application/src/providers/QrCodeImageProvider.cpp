#include "QrCodeImageProvider.h"

#include <QPainter>
#include <qrencode.h>

namespace XWallet::Providers {

constexpr quint32 DefaultWidth  = 200;
constexpr quint32 DefaultHeight = 200;

QrCodeImageProvider::QrCodeImageProvider(QObject* parent)
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage QrCodeImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    if (id.isEmpty())
    {
        qWarning() << Q_FUNC_INFO << "Empty QR code data provided";

        return QImage();
    }

    QString data = id.split(u'/', Qt::SkipEmptyParts).last();

    QRcode* code = QRcode_encodeString(data.toStdString().c_str(), 0, QRecLevel::QR_ECLEVEL_L, QRencodeMode::QR_MODE_8, 1);

    if (requestedSize.width() < code->width)
    {
        qWarning() << Q_FUNC_INFO << "Requested width is too small for drawing QR code";

        size->setWidth(DefaultWidth);
        size->setHeight(DefaultHeight);
    }
    else
    {
        size->setWidth(requestedSize.width());
        size->setHeight(requestedSize.height());
    }

    qreal scaleFactorX = qreal(size->width()) / qreal(code->width);
    qreal scaleFactorY = qreal(size->height()) / qreal(code->width);

    QImage image(*size, QImage::Format::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);

    QList<QRectF> rects;

    for (int y = 0; y < code->width; ++y)
    {
        for (int x = 0; x < code->width; ++x)
        {
            if (code->data[y * code->width + x] & 1)
            {
                rects.append(QRectF(x * scaleFactorX, y * scaleFactorY, scaleFactorX, scaleFactorY));
            }
        }
    }

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);

    painter.drawRects(rects);

    return image;
}

} // namespace XWallet::Providers
