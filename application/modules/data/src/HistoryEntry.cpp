#include "HistoryEntry.h"

#include "HistoryEntryData.h"

namespace Data {

HistoryEntry::HistoryEntry()
    : data(new HistoryEntryData)
{}

HistoryEntry::HistoryEntry(HistoryEntry&& other) noexcept      = default;
HistoryEntry::HistoryEntry(const HistoryEntry& other) noexcept = default;
HistoryEntry::~HistoryEntry()                                  = default;

HistoryEntry& HistoryEntry::operator= (const HistoryEntry& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool HistoryEntry::operator== (const HistoryEntry& other) const
{
    return data == other.data || *data == *other.data;
}

HistoryEntry::Direction HistoryEntry::direction() const
{
    return data->direction;
}

void HistoryEntry::setDirection(Direction direction)
{
    data->direction = direction;
}

Data::Currency HistoryEntry::currency() const
{
    return data->currency;
}

void HistoryEntry::setCurrency(const Data::Currency& currency)
{
    data->currency = currency;
}

QString HistoryEntry::hash() const
{
    return data->hash;
}

void HistoryEntry::setHash(const QString& hash)
{
    data->hash = hash;
}

QString HistoryEntry::amount() const
{
    return data->amount;
}

void HistoryEntry::setAmount(const QString& amount)
{
    data->amount = amount;
}

QString HistoryEntry::fee() const
{
    return data->fee;
}

void HistoryEntry::setFee(const QString& fee)
{
    data->fee = fee;
}

QList<QString> HistoryEntry::from() const
{
    return data->from;
}

void HistoryEntry::setFrom(const QList<QString>& from)
{
    data->from = from;
}

QList<QString> HistoryEntry::to() const
{
    return data->to;
}

void HistoryEntry::setTo(const QList<QString>& to)
{
    data->to = to;
}

QDateTime HistoryEntry::timestamp() const
{
    return data->timestamp;
}

void HistoryEntry::setTimestamp(const QDateTime& timestamp)
{
    data->timestamp = timestamp;
}

QString HistoryEntry::note() const
{
    return data->note;
}

void HistoryEntry::setNote(const QString& note)
{
    data->note = note;
}

bool HistoryEntry::success() const
{
    return data->success;
}

void HistoryEntry::setSuccess(bool success)
{
    data->success = success;
}

} // namespace Data
