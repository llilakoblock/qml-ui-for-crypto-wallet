#include "FacetecServer.h"

#include "WebChannelBackend.h"
#include "WebSocketTransport.h"

#include <QHttpServer>
#include <QTcpServer>
#include <QWebChannel>
#include <QWebSocket>

namespace XWallet::Biometric {

FacetecServer::FacetecServer(QObject* parent)
    : QObject(parent)
    , m_httpServer(new QHttpServer)
    , m_webChannel(new QWebChannel)
    , m_webSocketTransport(new WebSocketTransport)
    , m_webChannelBackend(new WebChannelBackend)
{
    m_httpServer->route("/websocket/<arg>", [](const QString& arg, const QHttpServerRequest& request) {
        qDebug() << Q_FUNC_INFO << request << arg;
        return QFuture<QHttpServerResponse>();
    });

    m_httpServer->route("/", []() {
        return QHttpServerResponse::fromFile(QStringLiteral(":/facetec/index.html"));
    });

    m_httpServer->route("/fonts/<arg>", [](const QUrl& url) {
        return QHttpServerResponse::fromFile(QStringLiteral(":/XWallet/Style/Fonts/Resources/%1").arg(url.path()));
    });

    m_httpServer->route("/<arg>", [](const QUrl& url) {
        return QHttpServerResponse::fromFile(QStringLiteral(":/facetec/%1").arg(url.path()));
    });

    const auto port = m_httpServer->listen(QHostAddress::LocalHost);

    if (!port)
    {
        qCritical() << Q_FUNC_INFO << "Failed to start server";
    }

    m_webChannel->registerObject(QStringLiteral("backend"), m_webChannelBackend.data());

    connect(m_httpServer.data(), &QHttpServer::newWebSocketConnection, this, [this]() {
        if (m_httpServer->hasPendingWebSocketConnections())
        {
            const auto& webSocket = QSharedPointer<QWebSocket>(m_httpServer->nextPendingWebSocketConnection().release());

            m_webSocketTransport->setSocket(webSocket);

            m_webChannel->connectTo(m_webSocketTransport.data());

            connect(m_webSocketTransport.data(), &WebSocketTransport::disconnected, this, [this]() {
                m_webChannel->disconnectFrom(m_webSocketTransport.data());
            });
        }
    });

    connect(m_webChannelBackend.data(), &WebChannelBackend::statusReceived, this, &FacetecServer::statusReceived);
}

FacetecServer::~FacetecServer() = default;

QUrl FacetecServer::url() const
{
    const auto& servers = m_httpServer->servers();

    if (servers.isEmpty() || servers.count() > 1 || !servers.at(0)->isListening())
    {
        qCritical() << Q_FUNC_INFO << "No server or multiple servers are listening";

        return {};
    }

    return QUrl(QStringLiteral("http://%1:%2").arg(servers.at(0)->serverAddress().toString()).arg(servers.at(0)->serverPort()));
}

void FacetecServer::showMainView()
{
    emit m_webChannelBackend->showMainView();
}

void FacetecServer::showEnrollView()
{
    emit m_webChannelBackend->showEnrollView();
}

void FacetecServer::showAuthenticateView()
{
    emit m_webChannelBackend->showAuthenticateView();
}

void FacetecServer::setUserHash(const QString& userHash)
{
    m_webChannelBackend->setUserHash(userHash);
}

} // namespace XWallet::Biometric
