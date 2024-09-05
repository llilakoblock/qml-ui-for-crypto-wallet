#include "AssetsReducer.h"

#include "singletones/Store.h"
#include "actions/NetworkServiceActions.h"
#include "misc/Visitor.h"

namespace XWallet::Reducers {

Data::Asset::Hash AssetsReducer::reduce(const Data::Asset::Hash& assets, const AssetActions::Action& action)
{
    const auto& visitor = Visitor {
        [assets](const AssetActions::AssetsUpdated& action) {
        for (const auto& asset : action.assets)
        {
            Store::instance().dispatch(DatabaseServiceActions::LoadQuotes { asset.type(), Data::Asset::Resolution::Max });

            const auto& requestQuote = NetworkServiceActions::RequestQuote(asset.type(), Data::Asset::Resolution::Max, NetworkServiceActions::Service::CoinGecko);

            Store::instance().dispatch(RootActions::AddTaskAction {
                requestQuote,
                RootActions::AddTaskAction::Type::Interval,
                1000 * 60 * 5,
                true });
        }

        return action.assets;
    },
        [assets](const AssetActions::AssetBalanceUpdated& action) {
        auto newAssets = assets;

        auto asset = assets.value(action.type);
        asset.setBalance(action.balance);

        newAssets.insert(action.type, asset);

        return newAssets;
    },
        [assets](const AssetActions::AssetFeeUpdated& action) {
        auto newAssets = assets;

        auto asset = assets.value(action.type);
        asset.setFee(action.fee);

        newAssets.insert(action.type, asset);

        return newAssets;
    },
        [assets](const AssetActions::AssetHistoryUpdated& action) {
        auto newAssets = assets;

        auto asset = assets.value(action.type);
        asset.setHistory(action.history);

        newAssets.insert(action.type, asset);

        return newAssets;
    },

        [assets](const AssetActions::AssetQuotesUpdated& action) {
        auto asset = assets.value(action.type);

        if (action.quotes.isEmpty() ||
            (!asset.quotes(action.resolution).isEmpty() && action.source == AssetActions::AssetQuotesUpdated::Source::Database))
        {
            return assets;
        }

        asset.setQuotes(action.quotes, action.resolution);

        if (asset.quote().timestamp() < action.quotes.last().timestamp())
        {
            asset.setQuote(action.quotes.last());
        }

        auto newAssets = assets;
        newAssets.insert(action.type, asset);

        return newAssets;
    },

    };

    const auto& result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
