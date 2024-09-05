#pragma once

#include "actions/OrderActions.h"
#include "data/Order.h"

namespace XWallet::Reducers {

class OrdersReducer
{
    Q_DISABLE_COPY_MOVE(OrdersReducer)

public:
    OrdersReducer() = delete;

    static QHash<QByteArray, Data::Order> reduce(const QHash<QByteArray, Data::Order>& orders,
                                                 const OrderActions::Action&           action);
};

} // namespace XWallet::Reducers
