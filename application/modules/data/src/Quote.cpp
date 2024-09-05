#include "data/Quote.h"

#include "QuoteData.h"

namespace Data {

Quote::Quote()
    : data(new QuoteData)
{}

Quote::Quote(Quote&& other) noexcept      = default;
Quote::Quote(const Quote& other) noexcept = default;
Quote::~Quote()                           = default;

Quote& Quote::operator= (const Quote& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Quote::operator== (const Quote& other) const
{
    return data == other.data || *data == *other.data;
}

QDateTime Quote::timestamp() const
{
    return data->timestamp;
}

void Quote::setTimestamp(const QDateTime& timestamp)
{
    data->timestamp = timestamp;
}

qreal Quote::price() const
{
    return data->price;
}

void Quote::setPrice(qreal price)
{
    data->price = price;
}

qreal Quote::capacity() const
{
    return data->capacity;
}

void Quote::setCapacity(qreal capacity)
{
    data->capacity = capacity;
}

} // namespace Data
