#include "NetworkService.h"

#include <QtConcurrent>
#include <QtNetwork/QNetworkReply>

namespace XWallet::Services {

constexpr qint32 MinimumProgress = 0;
constexpr qint32 MaximumProgress = 100;

NetworkService::NetworkService()
    : m_networkManager(new QNetworkAccessManager)
{
}

QFuture<QByteArray> NetworkService::get(const QUrl& url, const QHash<QByteArray, QByteArray>& headers)
{
    const auto& promise = QSharedPointer<QPromise<QByteArray>>::create();
    promise->setProgressRange(MinimumProgress, MaximumProgress);
    promise->start();

    QNetworkRequest request = QNetworkRequest(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    for (const auto& [key, value] : headers.asKeyValueRange())
    {
        request.setRawHeader(key, value);
    }

    const auto& reply = QSharedPointer<QNetworkReply>(m_networkManager->get(request));

    QtFuture::connect(reply.data(), &QNetworkReply::finished).then([reply, promise]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            throw reply->error();
        }

        promise->addResult(reply->readAll());
        promise->finish();
    }).onFailed([promise](QNetworkReply::NetworkError error) {
        promise->setException(NetworkException(NetworkException::Type::NetworkError, QObject::tr("Network error occured: %1").arg(error)));
        promise->finish();
    }).onFailed([promise] {
        promise->setException(NetworkException(NetworkException::Type::NetworkError, QObject::tr("Unhandled network error occured")));
        promise->finish();
    });

    QtFuture::connect(reply.data(), &QNetworkReply::downloadProgress).then([promise](std::tuple<qint64, qint64> args) {
        const auto& [bytesReceived, bytesTotal] = args;

        qint32 percent = 0;

        if (bytesTotal != 0)
        {
            percent = (bytesReceived * MaximumProgress) / bytesTotal;
        }

        promise->setProgressValue(percent);
    });

    return promise->future();
}

QFuture<QByteArray> NetworkService::post(const QUrl& url, const QByteArray& data, const QHash<QByteArray, QByteArray>& headers)
{
    const auto& promise = QSharedPointer<QPromise<QByteArray>>::create();
    promise->setProgressRange(MinimumProgress, MaximumProgress);
    promise->start();

    QNetworkRequest request = QNetworkRequest(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    for (const auto& [key, value] : headers.asKeyValueRange())
    {
        request.setRawHeader(key, value);
    }

    const auto& reply = QSharedPointer<QNetworkReply>(m_networkManager->post(request, data));

    QtFuture::connect(reply.data(), &QNetworkReply::finished).then([reply, promise]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            throw reply->error();
        }

        promise->addResult(reply->readAll());
        promise->finish();
    }).onFailed([promise](QNetworkReply::NetworkError error) {
        promise->setException(NetworkException(NetworkException::Type::NetworkError, QObject::tr("Network error occured: %1").arg(error)));
        promise->finish();
    }).onFailed([promise] {
        promise->setException(NetworkException(NetworkException::Type::NetworkError, QObject::tr("Unhandled network error occured")));
        promise->finish();
    });

    QtFuture::connect(reply.data(), &QNetworkReply::downloadProgress).then([promise](std::tuple<qint64, qint64> args) {
        const auto& [bytesReceived, bytesTotal] = args;

        qint32 percent = 0;

        if (bytesTotal != 0)
        {
            percent = (bytesReceived * MaximumProgress) / bytesTotal;
        }

        promise->setProgressValue(percent);
    });

    return promise->future();
}

} // namespace XWallet::Services
