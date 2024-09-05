#include "data/Balance.h"

#include "BalanceData.h"

namespace Data {

Balance::Balance()
    : data(new BalanceData)
{}

Balance::Balance(Balance&& other) noexcept      = default;
Balance::Balance(const Balance& other) noexcept = default;
Balance::~Balance()                             = default;

Balance& Balance::operator= (const Balance& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Balance::operator== (const Balance& other) const
{
    return data == other.data || *data == *other.data;
}

bool Balance::operator< (const Balance& other) const
{
    return total() < other.total();
}

bool Balance::operator> (const Balance& other) const
{
    return total() > other.total();
}

Int256 Balance::total() const
{
    return Int256(confirmed()) + unconfirmed();
}

UInt256 Balance::confirmed() const
{
    return data->confirmed;
}

void Balance::setConfirmed(const UInt256& confirmed)
{
    data->confirmed = confirmed;
}

Int256 Balance::unconfirmed() const
{
    return data->unconfirmed;
}

void Balance::setUnconfirmed(const Int256& unconfirmed)
{
    data->unconfirmed = unconfirmed;
}

} // namespace Data
