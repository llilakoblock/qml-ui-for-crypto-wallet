#pragma once

#include "NumericTypes.h"

#include <QSharedData>

namespace Data {

class CurrencyData : public QSharedData
{
public:
    inline bool operator== (const CurrencyData& other) const
    {
        return name == other.name &&
               asset == other.asset &&
               symbol == other.symbol &&
               code == other.code &&
               numberOfDecimal == other.numberOfDecimal &&
               smallCoinsInCoin == other.smallCoinsInCoin;
    }

    QString name;
    QString asset;
    QString symbol;
    QString code;

    quint32 numberOfDecimal { 0 };

    UInt256 smallCoinsInCoin { 0 };
};

} // namespace Data
