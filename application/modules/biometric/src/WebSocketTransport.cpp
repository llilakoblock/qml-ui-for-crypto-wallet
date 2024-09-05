#include "WebSocketTransport.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

namespace XWallet::Biometric {

WebSocketTransport::WebSocketTransport(QObject* parent)
    : QWebChannelAbstractTransport(parent)
{
}

void WebSocketTransport::setSocket(const QSharedPointer<QWebSocket>& socket)
{
    m_socket = socket;

    connect(socket.data(), &QWebSocket::textMessageReceived, this, &WebSocketTransport::textMessageReceived);
    connect(socket.data(), &QWebSocket::disconnected, this, &WebSocketTransport::disconnected);
}

WebSocketTransport::~WebSocketTransport() = default;

void WebSocketTransport::sendMessage(const QJsonObject& message)
{
    if (!m_socket)
    {
        qWarning() << Q_FUNC_INFO << "No socket to send message on";
        return;
    }

    QJsonDocument doc(message);
    m_socket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}

void WebSocketTransport::textMessageReceived(const QString& messageData)
{
    QJsonParseError error;
    QJsonDocument   message = QJsonDocument::fromJson(messageData.toUtf8(), &error);

    if (error.error)
    {
        qWarning() << Q_FUNC_INFO
                   << "Failed to parse text message as JSON object:" << messageData
                   << "Error is:" << error.errorString();
        return;
    }
    else if (!message.isObject())
    {
        qWarning() << Q_FUNC_INFO
                   << "Received JSON message that is not an object: " << messageData;
        return;
    }

    emit messageReceived(message.object(), this);
}

} // namespace XWallet::Biometric
