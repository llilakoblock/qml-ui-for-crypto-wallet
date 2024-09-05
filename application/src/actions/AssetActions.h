#pragma once

#include "data/Asset.h"
#include "data/Balance.h"

#include <variant>

namespace XWallet::AssetActions {

struct AssetsUpdated
{
    Data::Asset::Hash assets;
};

struct AssetBalanceUpdated
{
    Data::Asset::Type type { Data::Asset::Type::Unknown };
    Data::Balance     balance;
};

struct AssetFeeUpdated
{
    Data::Asset::Type type { Data::Asset::Type::Unknown };
    Data::Fee         fee;
};

struct AssetHistoryUpdated
{
    Data::Asset::Type         type { Data::Asset::Type::Unknown };
    QList<Data::HistoryEntry> history;
};

struct AssetQuotesUpdated
{
    enum class Source
    {
        Database,
        Network
    };

    Data::Asset::Type       type { Data::Asset::Type::Unknown };
    Data::Asset::Resolution resolution { Data::Asset::Resolution::Unknown };
    QList<Data::Quote>      quotes;
    Source                  source { Source::Database };
};

using Action = std::variant<AssetsUpdated,
                            AssetBalanceUpdated,
                            AssetFeeUpdated,
                            AssetHistoryUpdated,
                            AssetQuotesUpdated>;

inline QDebug operator<< (QDebug debug, const AssetsUpdated& action)
{
    QDebugStateSaver saver(debug);

    debug << "AssetsUpdated" << Qt::endl
          << "Count:" << action.assets.count() << Qt::endl;

    for (const auto& asset : action.assets)
    {
        debug << "---------------------------------------" << Qt::endl
              << "Type:" << asset.type() << Qt::endl
              << "Address:" << asset.address() << Qt::endl
              << "Is testnet:" << asset.isTestnet() << Qt::endl
              << "Currency name:" << asset.currency().name() << Qt::endl
              << "Currency asset:" << asset.currency().asset() << Qt::endl
              << "Currency symbol:" << asset.currency().symbol() << Qt::endl
              << "Currency code:" << asset.currency().code() << Qt::endl
              << "Balance confirmed:" << Data::toPrettyString(asset.balance().confirmed()) << Qt::endl
              << "Balance unconfirmed:" << Data::toPrettyString(asset.balance().unconfirmed()) << Qt::endl
              << "---------------------------------------" << Qt::endl;
    }

    return debug;
}

inline QDebug operator<< (QDebug debug, const AssetBalanceUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "AssetBalanceUpdated" << Qt::endl
          << "Type:" << item.type << Qt::endl
          << "Balance confirmed:" << Data::toPrettyString(item.balance.confirmed()) << Qt::endl
          << "Balance unconfirmed:" << Data::toPrettyString(item.balance.unconfirmed());

    return debug;
}

inline QDebug operator<< (QDebug debug, const AssetFeeUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "AssetFeeUpdated" << Qt::endl
          << "Type:" << item.type << Qt::endl
          << "Standart tx fee:" << Data::toPrettyString(item.fee.standardTxFee()) << Qt::endl
          << "Estimate fee:" << Data::toPrettyString(item.fee.estimateFee()) << Qt::endl
          << "Fund fee:" << Data::toPrettyString(item.fee.fundFee()) << Qt::endl
          << "Refund fee:" << Data::toPrettyString(item.fee.refundFee()) << Qt::endl
          << "Redeem fee:" << Data::toPrettyString(item.fee.redeemFee());

    return debug;
}

inline QDebug operator<< (QDebug debug, const AssetHistoryUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "AssetHistoryUpdated";

    return debug;
}

inline QDebug operator<< (QDebug debug, const AssetQuotesUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "AssetQuotesUpdated";

    return debug;
}

} // namespace XWallet::AssetActions
