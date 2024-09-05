#pragma once

#include <QFuture>
#include <QtNetwork/QNetworkAccessManager>

namespace XWallet::Services {

class NetworkException : public QException
{
public:
    enum class Type
    {
        Unknown,
        NetworkError,
        ParsingError,
        MalformedDataError,
    };

public:
    NetworkException(Type type = Type::Unknown, const QString& message = QString())
        : m_type(type)
        , m_message(message)
    {}

    void              raise() const override { throw *this; }
    NetworkException* clone() const override { return new NetworkException(*this); }

    Type    type() const { return m_type; }
    QString message() const { return m_message; }

private:
    Type    m_type = Type::Unknown;
    QString m_message;
};

class NetworkService
{
    Q_DISABLE_COPY_MOVE(NetworkService)

public:
    explicit NetworkService();

    QFuture<QByteArray> get(const QUrl& url, const QHash<QByteArray, QByteArray>& headers = {});
    QFuture<QByteArray> post(const QUrl& url, const QByteArray& data, const QHash<QByteArray, QByteArray>& headers = {});

private:
    QScopedPointer<QNetworkAccessManager> m_networkManager;
};

} // namespace XWallet::Services
