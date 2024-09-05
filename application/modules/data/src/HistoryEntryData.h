#pragma once

#include "Currency.h"
#include "HistoryEntry.h"

#include <QDateTime>
#include <QSharedData>

namespace Data {

class HistoryEntryData : public QSharedData
{
public:
    inline bool operator== (const HistoryEntryData& other) const
    {
        return direction == other.direction &&
               currency == other.currency &&
               hash == other.hash &&
               amount == other.amount &&
               fee == other.fee &&
               from == other.from &&
               to == other.to &&
               timestamp == other.timestamp &&
               note == other.note &&
               success == other.success;
    }

    HistoryEntry::Direction direction { HistoryEntry::Direction::Unknown };

    Data::Currency currency;

    QString hash;
    QString amount;
    QString fee;

    QList<QString> from;
    QList<QString> to;

    QDateTime timestamp;

    QString note;

    bool success { false };
};

} // namespace Data
