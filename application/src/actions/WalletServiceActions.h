#pragma once

#include "data/Asset.h"
#include "data/Order.h"

#include <QString>

#include <variant>

namespace XWallet::WalletServiceActions {

struct UnlockWallet
{
};

struct MakeTransaction
{
    Data::Asset::Type assetType;

    QString address;
    QString amount;
    QString fee;
};

struct PlaceOrder
{
    Data::Order order;
};

struct CancelOrder
{
    QString orderId;
};

using Action = std::variant<UnlockWallet,
                            MakeTransaction,
                            PlaceOrder,
                            CancelOrder>;

inline QDebug operator<< (QDebug debug, UnlockWallet item)
{
    QDebugStateSaver saver(debug);

    debug << "UnlockWallet" << Qt::endl;

    return debug;
}

inline QDebug operator<< (QDebug debug, const MakeTransaction& item)
{
    QDebugStateSaver saver(debug);

    debug << "MakeTransaction" << Qt::endl
          << "Asset type:" << item.assetType << Qt::endl
          << "Address:" << item.address << Qt::endl
          << "Amount:" << item.amount << Qt::endl
          << "Fee:" << item.fee;

    return debug;
}

inline QDebug operator<< (QDebug debug, const PlaceOrder& item)
{
    QDebugStateSaver saver(debug);

    debug << "PlaceOrder" << Qt::endl
          << "Bid asset:" << item.order.bidAssetType() << Qt::endl
          << "Ask asset:" << item.order.askAssetType();

    return debug;
}

inline QDebug operator<< (QDebug debug, const CancelOrder& item)
{
    QDebugStateSaver saver(debug);

    debug << "CancelOrder" << Qt::endl
          << "Order ID:" << item.orderId;

    return debug;
}

} // namespace XWallet::WalletServiceActions
