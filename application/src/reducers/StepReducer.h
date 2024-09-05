#pragma once

#include "actions/StepActions.h"
#include "data/Step.h"

namespace XWallet::Reducers {

class StepReducer
{
    Q_DISABLE_COPY_MOVE(StepReducer)

public:
    StepReducer() = delete;

    static Data::Step reduce(const Data::Step& step, const StepActions::Action& action);

private:
    static Data::Step finishScreen(const Data::Step& step, const StepActions::FinishScreen& action);
};

} // namespace XWallet::Reducers
