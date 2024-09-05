#pragma once

#include "actions/AssetActions.h"

namespace XWallet::Reducers {

class AssetsReducer
{
    Q_DISABLE_COPY_MOVE(AssetsReducer)

public:
    AssetsReducer() = delete;

    static Data::Asset::Hash reduce(const Data::Asset::Hash& assets, const AssetActions::Action& action);
};

} // namespace XWallet::Reducers
