#pragma once

#include "actions/ApplicationActions.h"
#include "actions/StepActions.h"
#include "actions/WalletActions.h"

#include <variant>

namespace XWallet::StateActions {

using Action = std::variant<ApplicationActions::Action,
                            WalletActions::Action,
                            StepActions::Action>;

} // namespace XWallet::StateActions
