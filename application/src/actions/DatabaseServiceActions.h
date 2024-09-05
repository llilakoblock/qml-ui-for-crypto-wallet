#pragma once

#include "data/Asset.h"
#include "data/HistoryEntry.h"
#include "data/Quote.h"

#include <QString>

#include <variant>

namespace XWallet::DatabaseServiceActions {

struct StoreQuotes
{
    Data::Asset::Type       type { Data::Asset::Type::Unknown };
    Data::Asset::Resolution resolution { Data::Asset::Resolution::Unknown };
    QList<Data::Quote>      quotes;
};

struct LoadQuotes
{
    Data::Asset::Type       type { Data::Asset::Type::Unknown };
    Data::Asset::Resolution resolution { Data::Asset::Resolution::Unknown };
};

struct StoreHistoryNotes
{
    Data::Asset::Type         type { Data::Asset::Type::Unknown };
    QList<Data::HistoryEntry> history;
};

struct LoadHistoryNotes
{
    Data::Asset::Type         type { Data::Asset::Type::Unknown };
    QList<Data::HistoryEntry> history;
};

using Action = std::variant<StoreQuotes,
                            LoadQuotes,
                            StoreHistoryNotes,
                            LoadHistoryNotes>;

// Debug output

inline QDebug operator<< (QDebug debug, const StoreQuotes& item)
{
    QDebugStateSaver saver(debug);

    debug << "StoreQuotes" << Qt::endl;

    return debug;
}

inline QDebug operator<< (QDebug debug, const LoadQuotes& item)
{
    QDebugStateSaver saver(debug);

    debug << "LoadQuotes" << Qt::endl;

    return debug;
}

inline QDebug operator<< (QDebug debug, const StoreHistoryNotes& item)
{
    QDebugStateSaver saver(debug);

    debug << "StoreHistory" << Qt::endl;

    return debug;
}

inline QDebug operator<< (QDebug debug, const LoadHistoryNotes& item)
{
    QDebugStateSaver saver(debug);

    debug << "LoadHistory" << Qt::endl;

    return debug;
}

} // namespace XWallet::DatabaseServiceActions
