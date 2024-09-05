#include "WalletReducer.h"

#include "actions/WalletActions.h"
#include "misc/Visitor.h"
#include "reducers/AssetsReducer.h"
#include "reducers/OrdersReducer.h"
#include "reducers/SwapsReducer.h"

namespace XWallet::Reducers {

Data::Wallet WalletReducer::reduce(const Data::Wallet& wallet, const WalletActions::Action& action)
{
    const auto& visitor = Visitor {
        [wallet](const WalletActions::WalletStateChanged& action) {
        auto newWallet = wallet;

        newWallet.setState(action.state);

        return newWallet;
    },
        [wallet](const AssetActions::Action& action) {
        const auto& assets = AssetsReducer::reduce(wallet.assets(), action);

        auto newWallet = wallet;
        newWallet.setAssets(assets);

        return newWallet;
    },
        [wallet](const OrderActions::Action& action) {
        const auto& orders = OrdersReducer::reduce(wallet.orders(), action);

        auto newWallet = wallet;
        newWallet.setOrders(orders);

        return newWallet;
    },
        [wallet](const SwapActions::Action& action) {
        const auto& swaps = SwapsReducer::reduce(wallet.swaps(), action);

        auto newWallet = wallet;
        newWallet.setSwaps(swaps);

        return newWallet;
    },

    };

    auto result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
