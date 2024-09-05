#pragma once

#include <QQuickImageProvider>

namespace XWallet::Providers {

class QrCodeImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    Q_DISABLE_COPY(QrCodeImageProvider)

public:
    QrCodeImageProvider(QObject* parent = nullptr);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;
};

} // namespace XWallet::Providers
