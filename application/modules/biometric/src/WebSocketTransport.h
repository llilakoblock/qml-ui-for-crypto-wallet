#pragma once

#include <QSharedPointer>
#include <QtWebChannel/QWebChannelAbstractTransport>

class QWebSocket;

namespace XWallet::Biometric {

class WebSocketTransport final : public QWebChannelAbstractTransport
{
    Q_OBJECT

public:
    explicit WebSocketTransport(QObject* parent = nullptr);
    ~WebSocketTransport() override;

    void setSocket(const QSharedPointer<QWebSocket>& socket);

    void sendMessage(const QJsonObject& message) override;

private:
    void textMessageReceived(const QString& message);

signals:
    void disconnected();

private:
    QSharedPointer<QWebSocket> m_socket;
};

} // namespace XWallet::Biometric
