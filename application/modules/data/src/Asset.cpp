#include "data/Asset.h"

#include "AssetData.h"

namespace Data {

Asset::Asset()
    : data(new AssetData)
{}

Asset::Asset(Asset&& other) noexcept      = default;
Asset::Asset(const Asset& other) noexcept = default;
Asset::~Asset()                           = default;

Asset& Asset::operator= (const Asset& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Asset::operator== (const Asset& other) const
{
    return data == other.data || *data == *other.data;
}

QString Asset::fiatAmountFromCoin(const QString& coinAmount) const
{
    if (coinAmount.isEmpty())
    {
        return QString();
    }

    QString cleanedCoinAmount = coinAmount;
    cleanedCoinAmount.remove(QRegularExpression("[^0-9.]"));

    Float100 coinAmountFloat(cleanedCoinAmount.toStdString());
    Float100 fiatAmountFloat = coinAmountFloat * quote().price();

    return toPrettyString(fiatAmountFloat, 2, 2);
}

Asset::Type Asset::type() const
{
    return data->type;
}

void Asset::setType(Type type)
{
    data->type = type;
}

Asset::Type Asset::parentType() const
{
    return data->parentType;
}

void Asset::setParentType(Type parentType)
{
    data->parentType = parentType;
}

QString Asset::address() const
{
    return data->address;
}

void Asset::setAddress(const QString& address)
{
    data->address = address;
}

Currency Asset::currency() const
{
    return data->currency;
}

void Asset::setCurrency(const Currency& currency)
{
    data->currency = currency;
}

Balance Asset::balance() const
{
    return data->balance;
}

void Asset::setBalance(const Balance& balance)
{
    data->balance = balance;
}

Fee Asset::fee() const
{
    return data->fee;
}

void Asset::setFee(const Fee& fee)
{
    data->fee = fee;
}

Quote Asset::quote() const
{
    return data->quote;
}

void Asset::setQuote(const Quote& quote)
{
    data->quote = quote;
}

QList<HistoryEntry> Asset::history() const
{
    return data->history;
}

void Asset::setHistory(const QList<HistoryEntry>& history)
{
    data->history = history;
}

QList<Quote> Asset::quotes(Resolution resolution) const
{
    switch (resolution)
    {
        case Resolution::Max:
            return data->quotesMaxResolution;
        case Resolution::Mid:
            return data->quotesMidResolution;
        case Resolution::Min:
            return data->quotesMinResolution;
        default:
            return {};
    }
}

void Asset::setQuotes(const QList<Quote>& quotes, Resolution resolution)
{
    switch (resolution)
    {
        case Resolution::Max:
            data->quotesMaxResolution = quotes;
            break;
        case Resolution::Mid:
            data->quotesMidResolution = quotes;
            break;
        case Resolution::Min:
            data->quotesMinResolution = quotes;
            break;
        default:
            break;
    }
}

bool Asset::isTestnet() const
{
    return data->isTestnet;
}

void Asset::setTestnet(bool isTestnet)
{
    data->isTestnet = isTestnet;
}

qreal Asset::difference() const
{
    if (data->quotesMaxResolution.isEmpty())
    {
        return 0;
    }

    return (data->quotesMaxResolution.last().price() - data->quotesMaxResolution.first().price()) * 100.0 / data->quotesMaxResolution.first().price();
}

Float100 Asset::smallCoinToBigCoinValue(const UInt256& value) const
{
    const Float100 smallCoinsInBigCoin = currency().smallCoinsInCoin().convert_to<Float100>();
    const Float100 valueBigCoinType    = value.convert_to<Float100>();

    return valueBigCoinType / smallCoinsInBigCoin;
}

Float100 Asset::smallCoinToBigCoinValue(const Int256& value) const
{
    const Float100 smallCoinsInBigCoin = currency().smallCoinsInCoin().convert_to<Float100>();
    const Float100 valueBigCoinType    = value.convert_to<Float100>();

    return valueBigCoinType / smallCoinsInBigCoin;
}

} // namespace Data
