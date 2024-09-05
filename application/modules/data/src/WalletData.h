#pragma once

#include "data/Asset.h"
#include "data/Order.h"
#include "data/Swap.h"
#include "data/Wallet.h"

#include <QSharedData>

namespace Data {

class WalletData : public QSharedData
{
public:
    inline bool operator== (const WalletData& other) const
    {
        return state == other.state &&
               assets == other.assets &&
               orders == other.orders &&
               swaps == other.swaps;
    }

    Wallet::State state { Wallet::State::UnknownState };

    QHash<Data::Asset::Type, Data::Asset> assets;

    QHash<QByteArray, Data::Order> orders;
    QHash<QByteArray, Data::Swap>  swaps;
};

} // namespace Data
