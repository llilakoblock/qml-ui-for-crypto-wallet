#pragma once

#include "Asset.h"
#include "Balance.h"
#include "Currency.h"
#include "Fee.h"
#include "HistoryEntry.h"
#include "Quote.h"

#include <QSharedData>
#include <QUrl>

namespace Data {

class AssetData : public QSharedData
{
public:
    inline bool operator== (const AssetData& other) const
    {
        return type == other.type &&
               parentType == other.parentType &&
               address == other.address &&
               currency == other.currency &&
               balance == other.balance &&
               fee == other.fee &&
               quote == other.quote &&
               isTestnet == other.isTestnet &&
               history == other.history &&
               quotesMaxResolution == other.quotesMaxResolution &&
               quotesMidResolution == other.quotesMidResolution &&
               quotesMinResolution == other.quotesMinResolution;
    }

    Asset::Type type { Asset::Type::Unknown };
    Asset::Type parentType { Asset::Type::Unknown };

    QString address;

    Currency currency;
    Balance  balance;
    Fee      fee;

    Quote quote;

    QList<HistoryEntry> history;

    QList<Quote> quotesMaxResolution;
    QList<Quote> quotesMidResolution;
    QList<Quote> quotesMinResolution;

    bool isTestnet;
};

} // namespace Data
