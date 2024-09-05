#include "SwapsReducer.h"

#include "misc/Visitor.h"

namespace XWallet::Reducers {

QHash<QByteArray, Data::Swap> SwapsReducer::reduce(const QHash<QByteArray, Data::Swap>& swaps, const SwapActions::Action& action)
{
    const auto& visitor = Visitor {
        [swaps](const SwapActions::SwapUpdated& action) {
        auto newSwaps = swaps;

        newSwaps.insert(action.swap.id(), action.swap);

        return newSwaps;
    },
        [swaps](const SwapActions::SwapsUpdated& action) {
        auto newSwaps = swaps;

        newSwaps.insert(action.swaps);

        return newSwaps;
    },

    };

    const auto& result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
