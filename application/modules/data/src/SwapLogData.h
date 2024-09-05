#pragma once

#include "data/SwapLog.h"

#include <QList>
#include <QSharedData>

namespace Data {

class SwapLogData : public QSharedData
{
public:
    inline bool operator== (const SwapLogData& other) const
    {
        return state == other.state &&
               text == other.text &&
               time == other.time;
    }

    SwapLog::State state { SwapLog::State::UnknownState };

    QString   text;
    QDateTime time;
};

} // namespace Data
