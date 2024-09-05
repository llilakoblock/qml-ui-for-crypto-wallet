#include "WalletMiddleware.h"

#include "singletones/Store.h"
#include "misc/Visitor.h"

namespace XWallet::Middleware {

WalletMiddleware::WalletMiddleware(QObject* parent)
    : QObject(parent)
    , m_walletService(new WalletService)
{
}

RootActions::Action WalletMiddleware::apply(const RootActions::Action& action)
{
    if (std::holds_alternative<WalletServiceActions::Action>(action))
    {
        const auto& walletServiceAction = std::get<WalletServiceActions::Action>(action);

        const auto& visitor = Visitor {
            [this](WalletServiceActions::UnlockWallet) {
            m_walletService->initWallet();

            Store::instance().dispatch(WalletActions::WalletStateChanged { Data::Wallet::State::UnlockedState });
            Store::instance().dispatch(ApplicationActions::ApplicationStateChanged { Data::Application::State::Ready });
        },
            [this](const WalletServiceActions::MakeTransaction& action) {
            m_walletService->makeTransaction(action.assetType, action.address, action.amount, action.fee).then([this, action](const std::string& txId) {
                emit transactionCompleted(action, QString::fromStdString(txId));
            }).onFailed([this, action](const QUnhandledException& e) {
                try
                {
                    if (e.exception())
                    {
                        std::rethrow_exception(e.exception());
                    }
                }
                catch (const std::exception& e)
                {
                    emit transactionFailed(action, QString::fromLatin1(e.what()));
                    return;
                }

                emit transactionFailed(action, "Unknown reason"_L1);
            }).onFailed([this, action] {
                emit transactionFailed(action, "Unknown reason"_L1);
            }).onCanceled([this, action] {
                emit transactionFailed(action, "Transaction canceled"_L1);
            });
        },
            [this](const WalletServiceActions::PlaceOrder& action) {
            m_walletService->placeOrder(action.order).then([this, action](const Data::Order& order) {
                emit orderCreated(action, order);
            });
        },
            [this](const WalletServiceActions::CancelOrder& action) {
            m_walletService->cancelOrder(action.orderId).then([this, action](const QString& orderId) {
                emit orderCanceled(action, orderId);
            });
        },
        };

        std::visit(visitor, walletServiceAction);

        return RootActions::EmptyAction {};
    }

    return action;
}

} // namespace XWallet::Middleware
