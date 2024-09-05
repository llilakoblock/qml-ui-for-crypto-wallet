#include "NetworkMiddleware.h"

#include "singletones/Store.h"
#include "misc/Visitor.h"

namespace XWallet::Middleware {

NetworkMiddleware::NetworkMiddleware(QObject* parent)
    : QObject(parent)
    , m_networkService(new Services::NetworkService)
{
}

RootActions::Action NetworkMiddleware::apply(const RootActions::Action& action)
{
    if (std::holds_alternative<NetworkServiceActions::Action>(action))
    {
        const auto& networkServiceAction = std::get<NetworkServiceActions::Action>(action);

        const auto& visitor = Visitor {
            [this](const NetworkServiceActions::RequestQuote& action) {
            m_networkService->get(action.url, action.headers).then(QtFuture::Launch::Async, [action](const QByteArray& responce) -> QList<Data::Quote> {
                return action.responceParser(responce);
            }).then([action](const QList<Data::Quote>& quotes) {
                if ((action.resolution == Data::Asset::Resolution::Max && quotes.count() < 250) ||
                    (action.resolution == Data::Asset::Resolution::Mid && quotes.count() < 2000) ||
                    (action.resolution == Data::Asset::Resolution::Min && quotes.count() < 280 /* VARA is not listed for full year, only since mid september */))
                {
                    throw Services::NetworkException(Services::NetworkException::Type::MalformedDataError, "Invalid quotes count");
                }

                const auto& quotesWithFixedTimestamp = fixQuotesTimestamp(quotes, action.resolution);

                Store::instance().dispatch(DatabaseServiceActions::StoreQuotes { action.assetType, action.resolution, quotesWithFixedTimestamp });

                Store::instance().dispatch(AssetActions::AssetQuotesUpdated {
                    action.assetType,
                    action.resolution,
                    quotesWithFixedTimestamp,
                    AssetActions::AssetQuotesUpdated::Source::Network });
            }).onFailed([action](const Services::NetworkException& e) {
                qWarning() << "Failed to get quotes for asset" << action.assetType
                           << "with resolution" << action.resolution
                           << "from" << action.service
                           << "due to" << e.message();

                Store::instance().dispatch(RootActions::AddTaskAction { action, RootActions::AddTaskAction::Type::OneShot, 1000 * 15, false });
            }).onFailed([action] {
                qWarning() << "Failed to get quotes for asset" << action.assetType
                           << "with resolution" << action.resolution
                           << "from" << action.service
                           << "due to unknown error";

                Store::instance().dispatch(RootActions::AddTaskAction { action, RootActions::AddTaskAction::Type::OneShot, 1000 * 2, false });
            });
        },
        };

        std::visit(visitor, networkServiceAction);

        return RootActions::EmptyAction {};
    }

    return action;
}

QList<Data::Quote> NetworkMiddleware::fixQuotesTimestamp(const QList<Data::Quote>& quotes, Data::Asset::Resolution resolution)
{
    QMultiMap<QDateTime, Data::Quote> roundedQuotesMap;

    const auto& roundTime = [](const QDateTime& datetime, qint32 interval) -> QDateTime {
        return QDateTime(datetime.date(), QTime(datetime.time().hour(), datetime.time().minute() / interval * interval));
    };

    const auto& minutesForResolution = [](Data::Asset::Resolution resolution) -> qint32 {
        switch (resolution)
        {
            case Data::Asset::Resolution::Max:
                return 5;
            case Data::Asset::Resolution::Mid:
                return 60;
            case Data::Asset::Resolution::Min:
                return 1440;
            default:
                return 1;
        }
    };

    for (const auto& quote : quotes)
    {
        Data::Quote fixedQuote = quote;

        QDateTime roundedTimestamp = roundTime(quote.timestamp(), minutesForResolution(resolution));

        fixedQuote.setTimestamp(roundedTimestamp);

        roundedQuotesMap.insert(roundedTimestamp, fixedQuote);
    }

    qint32 interval = minutesForResolution(resolution) * 60;

    QDateTime lastTimestamp = roundedQuotesMap.firstKey();

    while ((lastTimestamp = lastTimestamp.addSecs(interval)) < roundedQuotesMap.lastKey())
    {
        if (!roundedQuotesMap.contains(lastTimestamp))
        {
            const auto& uniqueKeys = roundedQuotesMap.uniqueKeys();

            auto lowerBound = std::lower_bound(uniqueKeys.constBegin(), uniqueKeys.constEnd(), lastTimestamp);

            const auto& nextKey = lowerBound == uniqueKeys.constEnd() ? QDateTime() : *lowerBound;
            const auto& prevKey = lowerBound == uniqueKeys.constEnd() ? QDateTime() : *--lowerBound;

            Data::Quote quote;

            if (roundedQuotesMap.count(prevKey) > 1)
            {
                QList<Data::Quote> quotes = roundedQuotesMap.values(prevKey);

                quote = quotes.takeLast();
                quote.setTimestamp(lastTimestamp);

                roundedQuotesMap.remove(prevKey);

                for (const auto& q : quotes)
                {
                    roundedQuotesMap.insert(prevKey, q);
                }
            }
            else if (roundedQuotesMap.count(nextKey) > 1)
            {
                QList<Data::Quote> quotes = roundedQuotesMap.values(nextKey);

                quote = quotes.takeFirst();
                quote.setTimestamp(lastTimestamp);

                roundedQuotesMap.remove(nextKey);

                for (const auto& q : quotes)
                {
                    roundedQuotesMap.insert(nextKey, q);
                }
            }
            else
            {
                quote = roundedQuotesMap.value(prevKey);
                quote.setTimestamp(lastTimestamp);
            }

            roundedQuotesMap.insert(lastTimestamp, quote);
        }
    }

    return roundedQuotesMap.values();
}

} // namespace XWallet::Middleware
