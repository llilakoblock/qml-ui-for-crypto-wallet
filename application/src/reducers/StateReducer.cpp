#include "StateReducer.h"

#include "misc/Visitor.h"
#include "reducers/ApplicationReducer.h"
#include "reducers/StepReducer.h"
#include "reducers/WalletReducer.h"

namespace XWallet::Reducers {

Data::State StateReducer::reduce(const Data::State& state, const StateActions::Action& action)
{
    const auto& visitor = Visitor {
        [state](const ApplicationActions::Action& action) {
        auto newState = state;

        auto newApplication = ApplicationReducer::reduce(state.application(), action);

        newState.setApplication(newApplication);

        return newState;
    },
        [state](const WalletActions::Action& action) {
        auto newState = state;

        auto newWallet = WalletReducer::reduce(state.wallet(), action);

        newState.setWallet(newWallet);

        return newState;
    },
        [state](const StepActions::Action& action) {
        auto newState = state;

        auto newStep = StepReducer::reduce(state.step(), action);

        newState.setStep(newStep);

        return newState;
    }
    };

    auto result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
