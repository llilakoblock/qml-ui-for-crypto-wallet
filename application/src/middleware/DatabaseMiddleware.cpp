#include "DatabaseMiddleware.h"

#include "Paths.h"
#include "singletones/Store.h"
#include "misc/Visitor.h"

namespace XWallet::Middleware {

using namespace Qt::Literals::StringLiterals;

namespace Tables {

constexpr auto Quotes  = "quotes"_L1;
constexpr auto History = "history"_L1;

} // namespace Tables

namespace Keys {

constexpr auto AssetType = "asset_type"_L1;

constexpr auto Resolution = "resolution"_L1;
constexpr auto Timestamp  = "timestamp"_L1;
constexpr auto Price      = "price"_L1;
constexpr auto Capacity   = "capacity"_L1;

constexpr auto Hash = "hash"_L1;
constexpr auto Note = "note"_L1;

} // namespace Keys

constexpr auto DatabaseFile = "ApplicationDatabase.db"_L1;

DatabaseMiddleware::DatabaseMiddleware(QObject* parent)
    : QObject(parent)
    , m_databaseService(new Services::DatabaseService)
{
    auto future = m_databaseService->establishConnection("%1/%2"_L1.arg(Paths::applicationDataDirectory(), DatabaseFile));

    future.then([this](bool success) {
        m_isConnected = success;
    });
}

RootActions::Action DatabaseMiddleware::apply(const RootActions::Action& action) const
{
    if (std::holds_alternative<DatabaseServiceActions::Action>(action))
    {
        const auto& databaseServiceAction = std::get<DatabaseServiceActions::Action>(action);

        const auto& visitor = Visitor {
            [this](const DatabaseServiceActions::StoreQuotes& action) {
            QList<QVariantMap> data;
            data.reserve(action.quotes.size());

            for (const auto& quote : action.quotes)
            {
                QVariantMap quoteData {
                    { Keys::Timestamp,  Services::DatabaseService::datetimeToString(quote.timestamp()) },
                    { Keys::Price,      quote.price()                                                  },
                    { Keys::Capacity,   quote.capacity()                                               },
                    { Keys::AssetType,  static_cast<qint32>(action.type)                               },
                    { Keys::Resolution, static_cast<qint32>(action.resolution)                         },
                };

                data.append(quoteData);
            }

            QVariantMap andBindables {
                { Keys::AssetType,  static_cast<qint32>(action.type)       },
                { Keys::Resolution, static_cast<qint32>(action.resolution) },
            };

            QFuture<bool> clearTableFuture = m_databaseService->removeItems(Tables::Quotes, andBindables);
            QFuture<bool> insertDataFuture = m_databaseService->replaceItemsList(Tables::Quotes, data);

            QList<QFuture<bool>> futures { clearTableFuture, insertDataFuture };

            QtFuture::whenAll(futures.begin(), futures.end()).then([this](const QList<QFuture<bool>>& results) {
                // for (const auto& result : results)
                // {
                //     std::visit([](auto&& future) { if (!future.result()){ qCritical() << "Failed to store quotes";} }, result);
                // }
            });
        },
            [this](const DatabaseServiceActions::LoadQuotes& action) {
            QStringList selections {
                Keys::Timestamp,
                Keys::Price,
                Keys::Capacity
            };

            QVariantMap bindables {
                { Keys::AssetType,  static_cast<qint32>(action.type)       },
                { Keys::Resolution, static_cast<qint32>(action.resolution) },
            };

            m_databaseService->loadItemsList(Tables::Quotes, selections, bindables).then([type = action.type, resolution = action.resolution](const QList<QVariantMap>& data) {
                QList<Data::Quote> quotes;
                quotes.reserve(data.size());

                for (const auto& item : data)
                {
                    Data::Quote quote;
                    quote.setTimestamp(Services::DatabaseService::stringToDatetime(item.value(Keys::Timestamp).toString()));
                    quote.setPrice(item.value(Keys::Price).toDouble());
                    quote.setCapacity(item.value(Keys::Capacity).toDouble());

                    quotes.append(quote);
                }

                Store::instance().dispatch(AssetActions::AssetQuotesUpdated {
                    type,
                    resolution,
                    quotes,
                    AssetActions::AssetQuotesUpdated::Source::Database });
            });
        },
            [this](const DatabaseServiceActions::StoreHistoryNotes& action) {
            QList<QVariantMap> data;
            data.reserve(action.history.size());

            for (const auto& histotyEntry : action.history)
            {
                QVariantMap quoteData {
                    { Keys::AssetType, static_cast<qint32>(action.type) },
                    { Keys::Hash,      histotyEntry.hash()              },
                    { Keys::Note,      histotyEntry.note()              },
                };

                data.append(quoteData);
            }

            QList<QString> indexedColumns { Keys::Hash };

            m_databaseService->upsertItemsList(Tables::History, indexedColumns, data).then([action](bool) {
                Store::instance().dispatch(AssetActions::AssetHistoryUpdated { action.type, action.history });
            });
        },
            [this](const DatabaseServiceActions::LoadHistoryNotes& action) {
            QStringList selections {
                Keys::Hash,
                Keys::Note,
            };

            QVariantMap bindables {
                { Keys::AssetType, static_cast<qint32>(action.type) },
            };

            m_databaseService->loadItemsList(Tables::History, selections, bindables).then([action](const QList<QVariantMap>& data) {
                QList<Data::HistoryEntry> history = action.history;

                for (const auto& item : data)
                {
                    QString hash = item.value(Keys::Hash).toString();

                    for (qsizetype i = 0; i < history.size(); i++)
                    {
                        if (history.at(i).hash() == hash)
                        {
                            auto newHistoryEntry = history.at(i);
                            newHistoryEntry.setNote(item.value(Keys::Note).toString());

                            history.replace(i, newHistoryEntry);
                            break;
                        }
                    }
                }

                Store::instance().dispatch(AssetActions::AssetHistoryUpdated { action.type, history });
            });
        },
        };

        std::visit(visitor, databaseServiceAction);

        return RootActions::EmptyAction {};
    }

    return action;
}

} // namespace XWallet::Middleware
