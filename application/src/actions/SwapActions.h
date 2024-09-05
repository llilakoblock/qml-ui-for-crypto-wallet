#pragma once

#include "data/Swap.h"

#include <variant>

namespace XWallet::SwapActions {

struct SwapUpdated
{
    Data::Swap swap;
};

struct SwapsUpdated
{
    QHash<QByteArray, Data::Swap> swaps;
};

using Action = std::variant<SwapUpdated, SwapsUpdated>;

inline QDebug operator<< (QDebug debug, const SwapUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "SwapUpdated" << Qt::endl
          << "Role:" << item.swap.role() << Qt::endl
          << "State:" << item.swap.state() << Qt::endl
          << "Logs count:" << item.swap.logs().count();

    return debug;
}

inline QDebug operator<< (QDebug debug, const SwapsUpdated& item)
{
    QDebugStateSaver saver(debug);

    debug << "SwapsUpdated";

    return debug;
}

} // namespace XWallet::SwapActions
