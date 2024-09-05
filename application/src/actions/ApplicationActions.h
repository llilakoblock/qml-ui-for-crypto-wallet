#pragma once

#include "data/Application.h"

#include <variant>

namespace XWallet::ApplicationActions {

struct ApplicationStateChanged
{
    Data::Application::State state { Data::Application::State::Unknown };
};

using Action = std::variant<ApplicationStateChanged>;

inline QDebug operator<< (QDebug debug, ApplicationStateChanged item)
{
    QDebugStateSaver saver(debug);

    debug << "ApplicationStateChanged" << Qt::endl
          << "New state:" << item.state;

    return debug;
}

} // namespace XWallet::ApplicationActions
