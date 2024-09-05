#pragma once

#include "NumericTypes.h"

#include <QSharedData>

namespace Data {

class BalanceData : public QSharedData
{
public:
    inline bool operator== (const BalanceData& other) const
    {
        return confirmed == other.confirmed &&
               unconfirmed == other.unconfirmed;
    }

    UInt256 confirmed;
    Int256  unconfirmed;
};

} // namespace Data
