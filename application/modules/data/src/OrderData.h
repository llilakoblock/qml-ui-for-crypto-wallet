#pragma once

#include "Asset.h"
#include "NumericTypes.h"
#include "Order.h"

#include <QList>
#include <QSharedData>

namespace Data {

class OrderData : public QSharedData
{
public:
    inline bool operator== (const OrderData& other) const
    {
        return id == other.id &&
               creator == other.creator &&
               bidAssetType == other.bidAssetType &&
               bidAmount == other.bidAmount &&
               askAssetType == other.askAssetType &&
               askAmount == other.askAmount;
    }

    QByteArray id;

    Order::Creator creator { Order::Creator::Unknown };

    Asset::Type bidAssetType;
    Float100    bidAmount { 0 };

    Asset::Type askAssetType;
    Float100    askAmount { 0 };
};

} // namespace Data
