#pragma once

#include <QHttpServer>
#include <QObject>
#include <QScopedPointer>
#include <QWebChannel>

class QHttpServer;
class QWebSocket;
class QWebChannel;

namespace XWallet::Biometric {

class WebSocketTransport;
class WebChannelBackend;

class FacetecServer final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(FacetecServer)

public:
    enum class Status
    {
        Unknown,
        EnrollSuccess,
        EnrollFail,
        EnrollCancel,
        AuthenticateSuccess,
        AuthenticateFail,
        AuthenticateCancel,
    };
    Q_ENUM(Status)

public:
    explicit FacetecServer(QObject* parent = nullptr);
    ~FacetecServer() override;

    QUrl url() const;

    void showMainView();
    void showEnrollView();
    void showAuthenticateView();

    void setUserHash(const QString& userHash);

signals:
    void statusReceived(Status status);

private:
    QScopedPointer<QHttpServer> m_httpServer;
    QScopedPointer<QWebChannel> m_webChannel;

    QScopedPointer<WebSocketTransport> m_webSocketTransport;
    QScopedPointer<WebChannelBackend>  m_webChannelBackend;
};

} // namespace XWallet::Biometric
