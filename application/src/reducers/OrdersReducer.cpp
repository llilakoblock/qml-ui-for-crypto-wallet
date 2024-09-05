#include "OrdersReducer.h"

#include "misc/Visitor.h"

namespace XWallet::Reducers {

QHash<QByteArray, Data::Order> OrdersReducer::reduce(const QHash<QByteArray, Data::Order>& orders,
                                                     const OrderActions::Action&           action)
{
    const auto& visitor = Visitor {
        [orders](const OrderActions::OrdersUpdated& action) {
        return action.orders;
    },
    };

    const auto& result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
