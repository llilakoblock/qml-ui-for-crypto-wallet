#pragma once

#include "actions/RootActions.h"
#include "services/WalletService.h"

namespace XWallet {

class Worker;

namespace Middleware {

class WalletMiddleware : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(WalletMiddleware)

public:
    WalletMiddleware(QObject* parent = nullptr);

private:
    RootActions::Action apply(const RootActions::Action& action);

signals:
    void transactionCompleted(const WalletServiceActions::MakeTransaction& action, const QString& txId);
    void transactionFailed(const WalletServiceActions::MakeTransaction& action, const QString& error);

    void orderCreated(const WalletServiceActions::PlaceOrder& action, const Data::Order& order);
    void orderCanceled(const WalletServiceActions::CancelOrder& action, const QString& orderId);

private:
    QScopedPointer<WalletService> m_walletService;

    friend class XWallet::Worker;
};

} // namespace Middleware
} // namespace XWallet
