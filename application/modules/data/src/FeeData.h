#pragma once

#include "NumericTypes.h"

#include <QSharedData>

namespace Data {

class FeeData : public QSharedData
{
public:
    inline bool operator== (const FeeData& other) const
    {
        return estimateFee == other.estimateFee &&
               fundFee == other.fundFee &&
               refundFee == other.refundFee &&
               redeemFee == other.redeemFee &&
               standardTxFee == other.standardTxFee;
    }

    UInt256 estimateFee;
    UInt256 fundFee;
    UInt256 refundFee;
    UInt256 redeemFee;
    UInt256 standardTxFee;
};

} // namespace Data
