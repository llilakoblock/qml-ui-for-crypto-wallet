#pragma once

#include "actions/AssetActions.h"
#include "actions/OrderActions.h"
#include "actions/SwapActions.h"
#include "data/Wallet.h"

#include <variant>

namespace XWallet::WalletActions {

struct WalletStateChanged
{
    Data::Wallet::State state;
};

using Action = std::variant<WalletStateChanged,
                            AssetActions::Action,
                            OrderActions::Action,
                            SwapActions::Action>;

inline QDebug operator<< (QDebug debug, WalletStateChanged item)
{
    QDebugStateSaver saver(debug);

    debug << "WalletStateChanged" << Qt::endl
          << "New state:" << item.state;

    return debug;
}

} // namespace XWallet::WalletActions
