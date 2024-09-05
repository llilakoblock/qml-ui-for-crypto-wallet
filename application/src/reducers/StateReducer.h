#pragma once

#include "actions/StateActions.h"
#include "data/State.h"

namespace XWallet::Reducers {

class StateReducer
{
    Q_DISABLE_COPY_MOVE(StateReducer)

public:
    StateReducer() = delete;

    static Data::State reduce(const Data::State& state, const StateActions::Action& action);
};

} // namespace XWallet::Reducers
