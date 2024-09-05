#pragma once

#include "actions/WalletActions.h"

namespace XWallet::Reducers {

class WalletReducer
{
    Q_DISABLE_COPY_MOVE(WalletReducer)

public:
    WalletReducer() = delete;

    static Data::Wallet reduce(const Data::Wallet& wallet, const WalletActions::Action& action);
};

} // namespace XWallet::Reducers
