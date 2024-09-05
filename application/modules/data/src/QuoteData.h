#pragma once

#include <QDateTime>
#include <QSharedData>

namespace Data {

class QuoteData : public QSharedData
{
public:
    inline bool operator== (const QuoteData& other) const
    {
        return timestamp == other.timestamp &&
               price == other.price &&
               capacity == other.capacity;
    }

    QDateTime timestamp;
    qreal     price;
    qreal     capacity;
};

} // namespace Data
