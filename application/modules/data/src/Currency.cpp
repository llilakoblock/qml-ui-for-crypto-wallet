#include "data/Currency.h"

#include "CurrencyData.h"

namespace Data {

Currency::Currency()
    : data(new CurrencyData)
{}

Currency::Currency(Currency&& other) noexcept      = default;
Currency::Currency(const Currency& other) noexcept = default;
Currency::~Currency()                              = default;

Currency& Currency::operator= (const Currency& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Currency::operator== (const Currency& other) const
{
    return data == other.data || *data == *other.data;
}

QString Currency::name() const
{
    return data->name;
}

void Currency::setName(const QString& name)
{
    data->name = name;
}

QString Currency::asset() const
{
    return data->asset;
}

void Currency::setAsset(const QString& asset)
{
    data->asset = asset;
}

QString Currency::symbol() const
{
    return data->symbol;
}

void Currency::setSymbol(const QString& symbol)
{
    data->symbol = symbol;
}

QString Currency::code() const
{
    return data->code;
}

void Currency::setCode(const QString& code)
{
    data->code = code;
}

quint32 Currency::numberOfDecimal() const
{
    return data->numberOfDecimal;
}

void Currency::setNumberOfDecimal(quint32 numberOfDecimals)
{
    data->numberOfDecimal = numberOfDecimals;
}

UInt256 Currency::smallCoinsInCoin() const
{
    return data->smallCoinsInCoin;
}

void Currency::setSmallCoinsInCoin(const UInt256& smallCoinsInCoin)
{
    data->smallCoinsInCoin = smallCoinsInCoin;
}

} // namespace Data
