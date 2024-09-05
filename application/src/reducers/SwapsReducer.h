#pragma once

#include "actions/SwapActions.h"
#include "data/Swap.h"

namespace XWallet::Reducers {

class SwapsReducer
{
    Q_DISABLE_COPY_MOVE(SwapsReducer)

public:
    SwapsReducer() = delete;

    static QHash<QByteArray, Data::Swap> reduce(const QHash<QByteArray, Data::Swap>& swaps, const SwapActions::Action& action);
};

} // namespace XWallet::Reducers
