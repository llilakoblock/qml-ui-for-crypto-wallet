#include "data/Order.h"

#include "OrderData.h"

namespace Data {

Order::Order()
    : data(new OrderData)
{
}

Order::Order(Order&& other) noexcept      = default;
Order::Order(const Order& other) noexcept = default;
Order::~Order()                           = default;

Order& Order::operator= (const Order& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Order::operator== (const Order& other) const
{
    return data == other.data || *data == *other.data;
}

QByteArray Order::id() const
{
    return data->id;
}

void Order::setId(const QByteArray& id)
{
    data->id = id;
}

Order::Creator Order::creator() const
{
    return data->creator;
}

void Order::setCreator(Creator creator)
{
    data->creator = creator;
}

Asset::Type Order::bidAssetType() const
{
    return data->bidAssetType;
}

void Order::setBidAssetType(Asset::Type bidAssetType)
{
    data->bidAssetType = bidAssetType;
}

Float100 Order::bidAmount() const
{
    return data->bidAmount;
}

void Order::setBidAmount(const Float100& bidAmount)
{
    data->bidAmount = bidAmount;
}

Asset::Type Order::askAssetType() const
{
    return data->askAssetType;
}

void Order::setAskAssetType(Asset::Type askAssetType)
{
    data->askAssetType = askAssetType;
}

Float100 Order::askAmount() const
{
    return data->askAmount;
}

void Order::setAskAmount(const Float100& askAmount)
{
    data->askAmount = askAmount;
}

} // namespace Data
