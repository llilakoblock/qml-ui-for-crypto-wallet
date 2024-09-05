#include "WebChannelBackend.h"

#include <QDebug>

namespace XWallet::Biometric {

WebChannelBackend::WebChannelBackend(QObject* parent)
    : QObject(parent)
{
}

void WebChannelBackend::setUserHash(const QString& userHash)
{
    if (m_userHash == userHash)
    {
        return;
    }

    m_userHash = userHash;
    emit userHashChanged();
}

void WebChannelBackend::enrollSuccess(const QString& passwordHash)
{
    qDebug() << "enrollSuccess" << passwordHash;

    emit statusReceived(FacetecServer::Status::EnrollSuccess);
}

void WebChannelBackend::enrollFail(const QString& passwordHash)
{
    qDebug() << "enrollFail" << passwordHash;

    emit statusReceived(FacetecServer::Status::EnrollFail);
}

void WebChannelBackend::enrollCancel(const QString& passwordHash)
{
    qDebug() << "enrollCancel" << passwordHash;

    emit statusReceived(FacetecServer::Status::EnrollCancel);
}

void WebChannelBackend::authSuccess(const QString& passwordHash)
{
    qDebug() << "authSuccess" << passwordHash;

    emit statusReceived(FacetecServer::Status::AuthenticateSuccess);
}

void WebChannelBackend::authFail(const QString& passwordHash)
{
    qDebug() << "authFail" << passwordHash;

    emit statusReceived(FacetecServer::Status::AuthenticateFail);
}

void WebChannelBackend::authCancel(const QString& passwordHash)
{
    qDebug() << "authCancel" << passwordHash;

    emit statusReceived(FacetecServer::Status::AuthenticateCancel);
}

} // namespace XWallet::Biometric
