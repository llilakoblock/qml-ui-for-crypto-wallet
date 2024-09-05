#pragma once

#include "data/Order.h"

#include <variant>

namespace XWallet::OrderActions {

struct OrdersUpdated
{
    QHash<QByteArray, Data::Order> orders;
};

using Action = std::variant<OrdersUpdated>;

inline QDebug operator<< (QDebug debug, const OrdersUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "OrdersUpdated" << Qt::endl
          << "Orders count:" << item.orders.count() << Qt::endl;

    for (const auto& order : item.orders)
    {
        debug << "Bid asset:" << order.bidAssetType() << Qt::endl
              << "Ask asset:" << order.askAssetType();
    }

    return debug;
}

} // namespace XWallet::OrderActions
