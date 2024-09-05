#pragma once

#include "data/Order.h"
#include "data/Swap.h"
#include "data/SwapLog.h"

#include <QList>
#include <QSharedData>

namespace Data {

class SwapData : public QSharedData
{
public:
    inline bool operator== (const SwapData& other) const
    {
        return id == other.id &&
               timestamp == other.timestamp &&
               myOrder == other.myOrder &&
               otherOrder == other.otherOrder &&
               role == other.role &&
               state == other.state &&
               logs == other.logs;
    }

    QByteArray id;

    QDateTime timestamp;

    Order myOrder;
    Order otherOrder;

    Swap::Role  role { Swap::Role::UnknownRole };
    Swap::State state { Swap::State::EmptyState };

    QList<SwapLog> logs;
};

} // namespace Data
