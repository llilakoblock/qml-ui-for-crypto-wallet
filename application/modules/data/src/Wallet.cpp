#include "Wallet.h"

#include "WalletData.h"

namespace Data {

Wallet::Wallet()
    : data(new WalletData)
{}

Wallet::Wallet(Wallet&& other) noexcept      = default;
Wallet::Wallet(const Wallet& other) noexcept = default;
Wallet::~Wallet()                            = default;

Wallet& Wallet::operator= (const Wallet& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Wallet::operator== (const Wallet& other) const
{
    return data == other.data || *data == *other.data;
}

Wallet::State Wallet::state() const
{
    return data->state;
}

void Wallet::setState(State state)
{
    data->state = state;
}

Asset::Hash Wallet::assets() const
{
    return data->assets;
}

void Wallet::setAssets(const Asset::Hash& assets)
{
    data->assets = assets;
}

QHash<QByteArray, Data::Order> Wallet::orders() const
{
    return data->orders;
}

void Wallet::setOrders(const QHash<QByteArray, Order>& orders)
{
    data->orders = orders;
}

QHash<QByteArray, Swap> Wallet::swaps() const
{
    return data->swaps;
}

void Wallet::setSwaps(const QHash<QByteArray, Swap>& swaps)
{
    data->swaps = swaps;
}

} // namespace Data
