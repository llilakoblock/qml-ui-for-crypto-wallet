#pragma once

#include "actions/RootActions.h"
#include "services/NetworkService.h"

namespace XWallet {

class Worker;

namespace Middleware {

class NetworkMiddleware : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(NetworkMiddleware)

public:
    NetworkMiddleware(QObject* parent = nullptr);

private:
    RootActions::Action apply(const RootActions::Action& action);

    static QList<Data::Quote> fixQuotesTimestamp(const QList<Data::Quote>& quotes, Data::Asset::Resolution resolution);

private:
    QScopedPointer<Services::NetworkService> m_networkService;

    friend class XWallet::Worker;
};

} // namespace Middleware
} // namespace XWallet
