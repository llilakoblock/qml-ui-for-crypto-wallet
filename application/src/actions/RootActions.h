#pragma once

#include "actions/DatabaseServiceActions.h"
#include "actions/NetworkServiceActions.h"
#include "actions/StateActions.h"
#include "actions/WalletServiceActions.h"

#include <variant>

namespace XWallet::RootActions {

using TaskAction = std::variant<WalletServiceActions::Action,
                                NetworkServiceActions::Action,
                                StateActions::Action>;

struct AddTaskAction
{
    enum class Type
    {
        OneShot,
        Interval,
    };

    TaskAction action;

    Type type      = Type::Interval;
    int  interval  = 1000;
    bool immediate = false;
};

struct EmptyAction
{
};

using Action = std::variant<EmptyAction,
                            AddTaskAction,
                            WalletServiceActions::Action,
                            NetworkServiceActions::Action,
                            DatabaseServiceActions::Action,
                            StateActions::Action>;

inline QDebug operator<< (QDebug debug, const AddTaskAction& item)
{
    QDebugStateSaver saver(debug);

    debug << "AddRepeatableAction" << Qt::endl
          << "Interval:" << item.interval << Qt::endl
          << "Immediate:" << item.immediate;

    return debug;
}

inline QDebug operator<< (QDebug debug, EmptyAction item)
{
    return debug;
}

} // namespace XWallet::RootActions
