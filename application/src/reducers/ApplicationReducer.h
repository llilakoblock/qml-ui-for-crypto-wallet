#pragma once

#include "actions/ApplicationActions.h"

namespace XWallet::Reducers {

class ApplicationReducer
{
    Q_DISABLE_COPY_MOVE(ApplicationReducer)

public:
    ApplicationReducer() = delete;

    static Data::Application reduce(const Data::Application& application, const ApplicationActions::Action& action);
};

} // namespace XWallet::Reducers
