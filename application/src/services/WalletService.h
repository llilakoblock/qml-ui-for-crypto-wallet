#pragma once

#include "data/Asset.h"
#include "data/Order.h"

#include <QFuture>
#include <QObject>

namespace freedom {
namespace orderbookclient {
class OrderBookClient;
}

class Wallet;

namespace orderbook {
namespace order {
struct Order;
}
} // namespace orderbook::order

namespace core {
class AddressHistory;

namespace api {
class Account;
class Balance;
} // namespace api

} // namespace core

namespace config {
class FreedomConfigurator;
}

namespace logger {
class FreedomLogger;
}

} // namespace freedom

namespace XWallet {

class WalletService final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(WalletService)

public:
    explicit WalletService(QObject* parent = nullptr);
    ~WalletService() override;

    void initWallet();

    QFuture<std::string> makeTransaction(Data::Asset::Type assetType, const QString& address, const QString& amount, const QString& fee);
    QFuture<Data::Order> placeOrder(const Data::Order& order);
    QFuture<QString>     cancelOrder(const QString& orderId);

private:
    void connectAccounts();
    void connectOrders();
    void connectSwaps();
    void loadDbBalances() const;

    Data::Order mapOrder(const freedom::orderbook::order::Order& order);

    Data::Asset::Hash mockAssets(const Data::Asset::Hash& existingAssets) const;

private:
    QScopedPointer<freedom::Wallet> m_wallet;

    QScopedPointer<freedom::config::FreedomConfigurator> m_configurator;
    QScopedPointer<freedom::logger::FreedomLogger>       m_logger;
};

} // namespace XWallet
