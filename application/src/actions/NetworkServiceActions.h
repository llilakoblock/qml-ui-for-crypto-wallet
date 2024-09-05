#pragma once

#include "data/Asset.h"

#include <QUrl>

#include <variant>

using namespace Qt::Literals::StringLiterals;

namespace XWallet::NetworkServiceActions {

Q_NAMESPACE

enum class Service : quint8
{
    Unknown,
    CoinGecko,
    CoinMarketCap,
};
Q_ENUM_NS(Service)

struct RequestQuote
{
    RequestQuote(Data::Asset::Type assetType, Data::Asset::Resolution resolution, Service service)
        : assetType(assetType)
        , resolution(resolution)
        , service(service)
    {
        switch (service)
        {
            case Service::CoinGecko:
            {
                QString assetId;

                switch (assetType)
                {
                    case Data::Asset::Type::Bitcoin:
                        assetId = "bitcoin"_L1;
                        break;
                    case Data::Asset::Type::Ethereum:
                        assetId = "ethereum"_L1;
                        break;
                    case Data::Asset::Type::Tether:
                        assetId = "tether"_L1;
                        break;
                    case Data::Asset::Type::Solana:
                        assetId = "solana"_L1;
                        break;
                    case Data::Asset::Type::Usdc:
                        assetId = "usd-coin"_L1;
                        break;
                    case Data::Asset::Type::Xrp:
                        assetId = "ripple"_L1;
                        break;
                    case Data::Asset::Type::Dogecoin:
                        assetId = "dogecoin"_L1;
                        break;
                    case Data::Asset::Type::Cardano:
                        assetId = "cardano"_L1;
                        break;
                    case Data::Asset::Type::Polkadot:
                        assetId = "polkadot"_L1;
                        break;
                    case Data::Asset::Type::VaraNetwork:
                        assetId = "vara-network"_L1;
                        break;
                    default:
                        break;
                }

                QString days;

                switch (resolution)
                {
                    case Data::Asset::Resolution::Max:
                        days = "1"_L1;
                        break;
                    case Data::Asset::Resolution::Mid:
                        days = "90"_L1;
                        break;
                    case Data::Asset::Resolution::Min:
                        days = "365"_L1;
                    default:
                        break;
                }

                url = QUrl("https://api.coingecko.com/api/v3/coins/%1/market_chart"_L1.arg(assetId));

                QUrlQuery query;
                query.addQueryItem("vs_currency"_L1, "usd"_L1);
                query.addQueryItem("days"_L1, days);

                url.setQuery(query);

                headers.insert("x-cg-demo-api-key", "CG-SKtgWadz3YjwtDb17t5ZzyCw");

                responceParser = [](const QByteArray& data) -> QList<Data::Quote> {
                    QJsonDocument document = QJsonDocument::fromJson(data);
                    QJsonObject   object   = document.object();

                    QJsonArray prices = object.value("prices"_L1).toArray();

                    QList<Data::Quote> result;
                    result.reserve(prices.size());

                    for (const auto& price : prices)
                    {
                        QJsonArray priceArray = price.toArray();

                        Data::Quote quote;
                        quote.setTimestamp(QDateTime::fromMSecsSinceEpoch(priceArray.at(0).toInteger()));
                        quote.setPrice(priceArray.at(1).toDouble());

                        result.append(quote);
                    }

                    return result;
                };

                break;
            }
            case Service::CoinMarketCap:
            {
                QString assetId;

                switch (assetType)
                {
                    case Data::Asset::Type::Bitcoin:
                        assetId = "1"_L1;
                        break;
                    case Data::Asset::Type::Ethereum:
                        assetId = "1027"_L1;
                        break;
                    case Data::Asset::Type::Tether:
                        assetId = "825"_L1;
                        break;
                    default:
                        break;
                }

                quint32 count { 0 };
                QString interval;

                QDateTime startTimestamp;
                QDateTime endTimestamp = QDateTime::currentDateTime();

                switch (resolution)
                {
                    case Data::Asset::Resolution::Max:
                        count          = 288;
                        interval       = QStringLiteral("5m");
                        startTimestamp = endTimestamp.addDays(-1);
                        break;
                    case Data::Asset::Resolution::Mid:
                        count          = 90;
                        interval       = QStringLiteral("hourly");
                        startTimestamp = endTimestamp.addDays(-count);
                        break;
                    case Data::Asset::Resolution::Min:
                        count          = 365;
                        interval       = QStringLiteral("daily");
                        startTimestamp = endTimestamp.addDays(-count);
                        break;
                    default:
                        break;
                }

                url = QUrl("https://pro-api.coinmarketcap.com/v3/cryptocurrency/quotes/historical"_L1);

                QUrlQuery query;
                query.addQueryItem("id"_L1, assetId);
                query.addQueryItem("convert"_L1, "USD"_L1);
                query.addQueryItem("time_start"_L1, startTimestamp.toString(Qt::ISODate));
                query.addQueryItem("time_end"_L1, endTimestamp.toString(Qt::ISODate));
                query.addQueryItem("interval"_L1, interval);
                query.addQueryItem("count"_L1, QString::number(count));

                url.setQuery(query);

                headers.insert("X-CMC_PRO_API_KEY", "7b364f66-006a-47e7-8cfd-3f2c6448109e");

                responceParser = [](const QByteArray& data) -> QList<Data::Quote> { return {}; };

                break;
            }
            default:
                break;
        }
    }

    Data::Asset::Type       assetType { Data::Asset::Type::Unknown };
    Data::Asset::Resolution resolution { Data::Asset::Resolution::Unknown };
    Service                 service { Service::Unknown };

    QUrl                          url;
    QHash<QByteArray, QByteArray> headers;

    std::function<QList<Data::Quote>(const QByteArray&)> responceParser = nullptr;
};

using Action = std::variant<RequestQuote>;

inline QDebug operator<< (QDebug debug, const RequestQuote& item)
{
    QDebugStateSaver saver(debug);

    debug << "RequestQuote" << Qt::endl
          << "Url:" << item.url << Qt::endl;

    return debug;
}

} // namespace XWallet::NetworkServiceActions
