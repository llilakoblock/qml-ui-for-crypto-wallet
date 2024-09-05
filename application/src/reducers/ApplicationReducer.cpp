#include "ApplicationReducer.h"

#include "misc/Visitor.h"

namespace XWallet::Reducers {

Data::Application ApplicationReducer::reduce(const Data::Application& application, const ApplicationActions::Action& action)
{
    const auto& visitor = Visitor {
        [application](ApplicationActions::ApplicationStateChanged action) {
        auto newApplication = application;

        newApplication.setState(action.state);

        return newApplication;
    },

    };

    auto result = std::visit(visitor, action);

    return result;
}

} // namespace XWallet::Reducers
