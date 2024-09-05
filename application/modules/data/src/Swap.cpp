#include "Swap.h"

#include "SwapData.h"

namespace Data {

Swap::Swap()
    : data(new SwapData)
{
}

Swap::Swap(Swap&& other) noexcept      = default;
Swap::Swap(const Swap& other) noexcept = default;
Swap::~Swap()                          = default;

Swap& Swap::operator= (const Swap& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Swap::operator== (const Swap& other) const
{
    return data == other.data || *data == *other.data;
}

QByteArray Swap::id() const
{
    return data->id;
}

void Swap::setId(const QByteArray& id)
{
    data->id = id;
}

QDateTime Swap::timestamp() const
{
    return data->timestamp;
}

void Swap::setTimestamp(const QDateTime& timestamp)
{
    data->timestamp = timestamp;
}

Order Swap::myOrder() const
{
    return data->myOrder;
}

void Swap::setMyOrder(const Order& order)
{
    data->myOrder = order;
}

Order Swap::otherOrder() const
{
    return data->otherOrder;
}

void Swap::setOtherOrder(const Order& order)
{
    data->otherOrder = order;
}

Swap::Role Swap::role() const
{
    return data->role;
}

void Swap::setRole(Role role)
{
    data->role = role;
}

Swap::State Swap::state() const
{
    return data->state;
}

void Swap::setState(State state)
{
    data->state = state;
}

QList<SwapLog> Swap::logs() const
{
    return data->logs;
}

void Swap::setLogs(const QList<SwapLog>& logs)
{
    data->logs = logs;
}

} // namespace Data
