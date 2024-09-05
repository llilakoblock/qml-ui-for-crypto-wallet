#pragma once

#include "FacetecServer.h"

#include <QObject>

class QWebChannel;
class QWebSocketServer;

namespace XWallet::Biometric {

class WebChannelBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userHash READ userHash WRITE setUserHash NOTIFY userHashChanged FINAL)

public:
    explicit WebChannelBackend(QObject* parent = nullptr);

    QString userHash() const { return m_userHash; }
    void    setUserHash(const QString& userHash);

public slots:
    void enrollSuccess(const QString& passwordHash);
    void enrollFail(const QString& passwordHash);
    void enrollCancel(const QString& passwordHash);

    void authSuccess(const QString& passwordHash);
    void authFail(const QString& passwordHash);
    void authCancel(const QString& passwordHash);

signals:
    void userHashChanged();

    void showMainView();
    void showEnrollView();
    void showAuthenticateView();

    void statusReceived(FacetecServer::Status status);

private:
    QString m_userHash;
};

} // namespace XWallet::Biometric
