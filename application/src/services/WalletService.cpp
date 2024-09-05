#include "WalletService.h"

#include "actions/AssetActions.h"
#include "actions/DatabaseServiceActions.h"
#include "actions/WalletActions.h"
#include "addresshistory.h"
#include "bitcoinaccount.h"
#include "erc20account.h"
#include "ethereumaccount.h"
#include "feeinfo.h"
#include "keyfile.h"
#include "orderbookclient.h"
#include "singletones/Store.h"
#include "sqldatabase.h"
#include "substrateaccount.h"
#include "swapprocessor.h"
#include "freedom_logger.h"
#include "freedomconfig.h"
#include "freedomdatabase.h"
#include "wallet.h"

namespace XWallet {

WalletService::WalletService(QObject* parent)
    : QObject(parent)
    , m_configurator(new freedom::config::FreedomConfigurator)
{
    QtFuture::connect(&Store::instance(), &Store::launched).then([]() {
        if (KeyFile::IsKeyFileExists())
        {
            Store::instance().dispatch(WalletActions::WalletStateChanged { Data::Wallet::State::LockedState });
            Store::instance().dispatch(StepActions::SetInitScreen { Data::Step::Screen::Unlock });
        }
        else
        {
            Store::instance().dispatch(WalletActions::WalletStateChanged { Data::Wallet::State::UnexistState });
            Store::instance().dispatch(StepActions::SetInitScreen { Data::Step::Screen::Welcome });
        }
    });

    freedom::config::CommonSettings settings;
    m_configurator->GetCommonConfig(settings);

    freedom::config::DatabaseConfig config;
    m_configurator->GetDatabaseConfig(config);

    freedom::core::database::SqlDatabaseConfig sqlconfig(config);
    sqlconfig.SetDatabasePath(Paths::applicationDataDirectory());

    m_logger.reset(new freedom::logger::FreedomLogger(settings.LogLevel));

    freedom::core::database::SqlDatabase::SetDatabaseConfig(sqlconfig);

    if (!freedom::core::database::FreedomDatabase::CheckDatabaseExists())
    {
        freedom::core::database::FreedomDatabase::CreateDatabase();
    }

    freedom::core::database::SqlDatabase::Remove();
}

WalletService::~WalletService() = default;

void WalletService::initWallet()
{
    m_wallet.reset(new freedom::Wallet(m_configurator.data(), this));

    auto result = m_wallet->StartObservers().result();

    connectAccounts();
}

QFuture<std::string> WalletService::makeTransaction(Data::Asset::Type assetType, const QString& address, const QString& amount, const QString& fee)
{
    const auto& accounts = m_wallet->GetAccounts();

    for (const auto& account : accounts)
    {
        if (assetType == Data::Asset::Type::Bitcoin && account->IsInstanceOfBitcoinLikeAccount())
        {
            const auto& bitcoinAccount = account->AsBitcoinLikeAccount();

            return bitcoinAccount->BuildAndSendTransaction(amount, address);
        }
        else if (assetType == Data::Asset::Type::Ethereum && account->IsInstanceOfEthereumLikeAccount())
        {
            const auto& ethereumAccount = account->AsEthereumLikeAccount();

            return ethereumAccount->BuildAndSendTransaction(amount, address);
        }
        else if (assetType == Data::Asset::Type::Tether && account->IsInstanceOfERC20LikeAccount())
        {
            const auto& erc20Account = account->AsErc20LikeAccount();

            return erc20Account->BuildAndSendTransaction(amount, address);
        }
        else if (assetType == Data::Asset::Type::VaraNetwork && account->IsInstanceOfSubstrateLikeAccount())
        {
            const auto& substrate_account = account->AsSubstrateAccount();

            return substrate_account->BuildAndSendTransaction(amount, address);
        }
    }

    qCritical() << Q_FUNC_INFO << "Account not found";

    return QFuture<std::string>();
}

QFuture<Data::Order> WalletService::placeOrder(const Data::Order& order)
{
    // TODO: UGLY AS FUCK

    Data::Asset bidAsset = Store::instance().state().wallet().assets().value(order.bidAssetType());
    Data::Asset askAsset = Store::instance().state().wallet().assets().value(order.askAssetType());

    freedom::order::Asset serviceBidAsset;
    serviceBidAsset.nominal_amount = order.bidAmount();
    serviceBidAsset.name           = bidAsset.currency().asset().toLower().toStdString();
    serviceBidAsset.ledger.name    = bidAsset.currency().name().toLower().toStdString();

    freedom::order::Asset serviceAskAsset;
    serviceAskAsset.nominal_amount = order.askAmount();
    serviceAskAsset.name           = askAsset.currency().asset().toLower().toStdString();
    serviceAskAsset.ledger.name    = askAsset.currency().name().toLower().toStdString();

    freedom::order::Order orderToSend;
    orderToSend.alpha_asset       = serviceBidAsset;
    orderToSend.beta_asset        = serviceAskAsset;
    orderToSend.alpha_asset_price = bidAsset.quote().price() * orderToSend.alpha_asset.nominal_amount;
    orderToSend.beta_asset_price  = askAsset.quote().price() * orderToSend.beta_asset.nominal_amount;
    orderToSend.user_slippage     = "1";

    orderToSend.alpha_asset_price.value().precision(2);
    orderToSend.beta_asset_price.value().precision(2);

    freedom::core::api::IOrderBookProvider* order_book_provider = m_wallet->GetOrderClient();

    const auto& future = QtFuture::connect(order_book_provider->m_signals.get(), &freedom::core::api::IOrderBookSignals::OrderCreated).then([this](const Order& serviceOrder) {
        Data::Order order = mapOrder(serviceOrder);

        return order;
    });

    order_book_provider->AddOrderRequest(orderToSend);

    return future;
}

QFuture<QString> WalletService::cancelOrder(const QString& orderId)
{
    freedom::core::api::IOrderBookProvider* order_book_provider = m_wallet->GetOrderClient();

    const auto& future = QtFuture::connect(order_book_provider->m_signals.get(), &freedom::core::api::IOrderBookSignals::OrderDeleted).then([this](const freedom::orderbookclient::OrderId& serviceOrderId) {
        return QString::fromStdString(serviceOrderId);
    });

    order_book_provider->DeleteOrderRequest(orderId.toStdString());

    return future;
}

void WalletService::connectAccounts()
{
    if (!m_wallet)
    {
        qCritical() << Q_FUNC_INFO << "Wallet is not initialized";
        return;
    }

    const auto& assetType = [](QSharedPointer<freedom::Account> account) -> Data::Asset::Type {
        if (account->IsInstanceOfBitcoinLikeAccount())
        {
            return Data::Asset::Type::Bitcoin;
        }
        else if (account->IsInstanceOfEthereumLikeAccount())
        {
            return Data::Asset::Type::Ethereum;
        }
        else if (account->IsInstanceOfERC20LikeAccount())
        {
            return Data::Asset::Type::Tether;
        }
        else if (account->IsInstanceOfSubstrateLikeAccount())
        {
            return Data::Asset::Type::VaraNetwork;
        }

        return Data::Asset::Type::Unknown;
    };

    const auto& mergeHistory = [](const QList<Data::HistoryEntry>& oldHistory, const QList<Data::HistoryEntry>& newHistory) {
        QList<Data::HistoryEntry> merged = oldHistory;

        for (qsizetype i = 0; i < newHistory.size(); ++i)
        {
            bool found = false;

            const auto& newHistoryEntry = newHistory.at(i);

            for (qsizetype j = 0; j < oldHistory.size(); ++j)
            {
                const auto& oldHistoryEntry = oldHistory.at(j);

                if (oldHistoryEntry.hash() == newHistoryEntry.hash())
                {
                    merged.replace(j, newHistoryEntry);

                    found = true;

                    break;
                }
            }

            if (!found)
            {
                merged.append(newHistoryEntry);
            }
        }

        return merged;
    };

    const auto& mapHistory = [](QSharedPointer<freedom::Account> account, const Data::Currency currency, const freedom::core::AddressHistory& addressHistory) -> QList<Data::HistoryEntry> {
        QList<Data::HistoryEntry> result;

        const auto& history = addressHistory.history;

        for (const auto& entry : history)
        {
            Data::HistoryEntry historyEntry;

            switch (entry.direction)
            {
                case freedom::core::AddressHistoryEntry::Direction::TXIN:
                    historyEntry.setDirection(Data::HistoryEntry::Direction::Income);
                    break;
                case freedom::core::AddressHistoryEntry::Direction::TXOUT:
                    historyEntry.setDirection(Data::HistoryEntry::Direction::Outgo);
                    break;
                default:
                    break;
            }

            historyEntry.setCurrency(currency);
            historyEntry.setHash(QString::fromStdString(entry.txhash));
            historyEntry.setAmount(Data::toPrettyString(Data::Float100(entry.amount), currency.numberOfDecimal()));
            historyEntry.setFee(Data::toPrettyString(Data::Float100(entry.fee), currency.numberOfDecimal()));
            historyEntry.setTimestamp(QDateTime::fromSecsSinceEpoch(entry.timestamp));
            historyEntry.setSuccess(entry.success);

            QList<QString> fromList;
            if (entry.from.has_value())
            {
                for (const auto& fromEntry : entry.from.value())
                {
                    fromList.append(QString::fromStdString(fromEntry));
                }
            }

            historyEntry.setFrom(fromList);

            QList<QString> toList;
            if (entry.to.has_value())
            {
                for (const auto& toEntry : entry.to.value())
                {
                    toList.append(QString::fromStdString(toEntry));
                }
            }

            historyEntry.setTo(toList);

            result.append(historyEntry);
        }

        return result;
    };

    const auto& accounts = m_wallet->GetAccounts();

    Data::Asset::Hash assets;

    for (const auto& account : accounts)
    {
        const auto& currencyUnit = account->GetCurrencyUnit();

        auto assetTypeEnum = assetType(account);

        Data::Asset    asset;
        Data::Currency currency;

        currency.setAsset(currencyUnit.asset);
        currency.setCode(currencyUnit.code);
        currency.setName(currencyUnit.name);
        currency.setSymbol(currencyUnit.symbol);
        currency.setNumberOfDecimal(currencyUnit.numberOfDecimal);
        currency.setSmallCoinsInCoin(currencyUnit.smallCoinsInCoin);

        // Special case for Tether
        if (assetTypeEnum == Data::Asset::Type::Tether)
        {
            currency.setName("Tether");
            currency.setSymbol("USDT");

            asset.setParentType(Data::Asset::Type::Ethereum);
        }

        asset.setAddress(QString::fromStdString(account->GetAddress()));
        asset.setTestnet(account->IsTestNetwork());
        asset.setCurrency(currency);
        asset.setType(assetTypeEnum);

        assets.insert(asset.type(), asset);

        connect(account.data(), &freedom::Account::BalanceUpdated, this, [account, assetType](const freedom::core::api::Balance& balance) {
            AssetActions::AssetBalanceUpdated action;
            action.balance.setConfirmed(balance.confirmed);
            action.balance.setUnconfirmed(balance.unconfirmed);

            action.type = assetType(account);

            Store::instance().dispatch(action);
        });

        connect(account.data(), &freedom::Account::NetworkFeeUpdated, this, [account, assetType](const freedom::core::FeeInfo& feeInfo) {
            Data::Fee fee;
            fee.setEstimateFee(feeInfo.GetEstimateFee());
            fee.setFundFee(feeInfo.GetFundFee());
            fee.setRefundFee(feeInfo.GetRefundFee());
            fee.setRedeemFee(feeInfo.GetRedeemFee());
            fee.setStandardTxFee(feeInfo.GetStandardTxFee());

            AssetActions::AssetFeeUpdated action;
            action.fee  = fee;
            action.type = assetType(account);

            Store::instance().dispatch(action);
        });

        connect(account.data(), &freedom::Account::HistoryUpdated, this, [mapHistory, mergeHistory, assetType, account, currency](const freedom::core::AddressHistory& addressHistory) {
            Data::Asset::Type type = assetType(account);

            const auto& oldHistory = Store::instance().state().wallet().assets().value(type).history();
            const auto& newHistory = mapHistory(account, currency, addressHistory);

            DatabaseServiceActions::LoadHistoryNotes action;
            action.history = mergeHistory(oldHistory, newHistory);
            action.type    = type;

            Store::instance().dispatch(action);
        });

        account->GetHistory().then([mapHistory, assetType, account, currency](const freedom::core::AddressHistory& addressHistory) {
            DatabaseServiceActions::LoadHistoryNotes action;
            action.history = mapHistory(account, currency, addressHistory);
            action.type    = assetType(account);

            Store::instance().dispatch(action);
        }).onFailed([assetType, account](const std::exception& e) {
            qCritical() << "Get history error"
                        << "Asset type:" << assetType(account) << QString::fromLatin1(e.what());
        }).onFailed([assetType, account] {
            qCritical() << "Get history error"
                        << "Asset type:" << assetType(account) << "Unknown exception";
        });

        account->UpdateBalance();
    }

    assets = mockAssets(assets);

    Store::instance().dispatch(AssetActions::AssetsUpdated { assets }).then([this]() {
        connectOrders();
        connectSwaps();
        loadDbBalances();
    });
}

void WalletService::connectOrders()
{
    if (!m_wallet)
    {
        qCritical() << Q_FUNC_INFO << "Wallet is not initialized";
        return;
    }

    const auto& updateOrders = [this](const Orders& ordersList) {
        QHash<QByteArray, Data::Order> orders;

        for (const auto& serviceOrder : ordersList)
        {
            Data::Order order = mapOrder(serviceOrder);

            order.setId(QByteArray::fromStdString(serviceOrder.id));

            orders.insert(order.id(), order);
        }

        Store::instance().dispatch(OrderActions::OrdersUpdated { orders });
    };

    freedom::core::api::IOrderBookProvider* order_book_provider = m_wallet->GetOrderClient();

    updateOrders(order_book_provider->GetOrders());

    //////////////////////////////////////////

    connect(order_book_provider->m_signals.get(), &freedom::core::api::IOrderBookSignals::OrdersListChanged, this, [order_book_provider, updateOrders]() {
        updateOrders(order_book_provider->GetOrders());
    });

    connect(order_book_provider->m_signals.get(), &freedom::core::api::IOrderBookSignals::OrdersMatched, m_wallet->GetSwapProcessor(), &freedom::SwapProcessor::OnMatchedOrders);

    // connect(m_wallet->GetSwapProcessor(), &freedom::SwapProcessor::SwapStarted, this, [this](const freedom::Swap& swap) {
    //     if (swap.GetRole() == freedom::Swap::Role::Alice)
    //     {
    //         m_wallet->GetOrderClient()->DeleteOrderRequest(swap.GetMakerOrder().id);
    //     }
    //     else
    //     {
    //         m_wallet->GetOrderClient()->DeleteOrderRequest(swap.GetTakerOrder().id);
    //     }
    // });
}

void WalletService::connectSwaps()
{
    if (!m_wallet)
    {
        qCritical() << Q_FUNC_INFO << "Wallet is not initialized";
        return;
    }

    const auto& mapSwapRole = [](freedom::Swap::Role role) -> Data::Swap::Role {
        switch (role)
        {
            case freedom::Swap::Role::Alice:
                return Data::Swap::Role::AliceRole;
            case freedom::Swap::Role::Bob:
                return Data::Swap::Role::BobRole;
            default:
                return Data::Swap::Role::UnknownRole;
        }
    };

    const auto& mapSwapState = [](freedom::Swap::State state) -> Data::Swap::State {
        switch (state)
        {
            case freedom::Swap::State::Empty:
                return Data::Swap::State::EmptyState;
            case freedom::Swap::State::ReadyForFund:
                return Data::Swap::State::ReadyForFundState;
            case freedom::Swap::State::Funding:
                return Data::Swap::State::FundingState;
            case freedom::Swap::State::FundFailed:
                return Data::Swap::State::FundFailedState;
            case freedom::Swap::State::FundSucceed:
                return Data::Swap::State::FundSucceedState;
            case freedom::Swap::State::ReadyForRedeem:
                return Data::Swap::State::ReadyForRedeemState;
            case freedom::Swap::State::Redeeming:
                return Data::Swap::State::RedeemingState;
            case freedom::Swap::State::RedeemFailed:
                return Data::Swap::State::RedeemFailedState;
            case freedom::Swap::State::RedeemSucceed:
                return Data::Swap::State::RedeemSucceedState;
            case freedom::Swap::State::ReadyForRefund:
                return Data::Swap::State::ReadyForRefundState;
            case freedom::Swap::State::Refunding:
                return Data::Swap::State::RefundingState;
            case freedom::Swap::State::RefundFailed:
                return Data::Swap::State::RefundFailedState;
            case freedom::Swap::State::RefundSucceed:
                return Data::Swap::State::RefundSucceedState;
            case freedom::Swap::State::Finished:
                return Data::Swap::State::FinishedState;
            case freedom::Swap::State::Declined:
                return Data::Swap::State::DeclinedState;
            case freedom::Swap::State::Error:
                return Data::Swap::State::ErrorState;
        }
    };

    const auto& mapSwapLogState = [](freedom::SwapLog::State state) -> Data::SwapLog::State {
        switch (state)
        {
            case freedom::SwapLog::State::None:
                return Data::SwapLog::State::UnknownState;
            case freedom::SwapLog::State::InProgress:
                return Data::SwapLog::State::InProgressState;
            case freedom::SwapLog::State::Done:
                return Data::SwapLog::State::DoneState;
            case freedom::SwapLog::State::Failed:
                return Data::SwapLog::State::FailedState;
        }
    };

    const auto& mapSwap = [this, mapSwapRole, mapSwapState, mapSwapLogState](const freedom::Swap& serviceSwap) -> Data::Swap {
        Data::Swap swap;
        swap.setId(QByteArray::fromStdString(serviceSwap.id));
        swap.setTimestamp(serviceSwap.GetTimestamp());
        swap.setRole(mapSwapRole(serviceSwap.GetRole()));
        swap.setState(mapSwapState(serviceSwap.GetState()));

        Data::Order makerOrder = mapOrder(serviceSwap.GetMakerOrder());
        Data::Order takerOrder = mapOrder(serviceSwap.GetTakerOrder());

        if (swap.role() == Data::Swap::Role::AliceRole)
        {
            swap.setMyOrder(makerOrder);
            swap.setOtherOrder(takerOrder);
        }
        else if (swap.role() == Data::Swap::Role::BobRole)
        {
            swap.setMyOrder(takerOrder);
            swap.setOtherOrder(makerOrder);
        }
        else
        {
            qCritical() << Q_FUNC_INFO << "Unknown swap role";
        }

        QList<Data::SwapLog> swapLogs;

        const auto& swapLogsList = serviceSwap.GetLogs();

        for (const auto& serviceSwapLog : swapLogsList)
        {
            Data::SwapLog swapLog;
            swapLog.setState(mapSwapLogState(serviceSwapLog.GetState()));
            swapLog.setTime(serviceSwapLog.GetTimestamp());
            swapLog.setText(serviceSwapLog.GetLog());

            swapLogs.append(swapLog);
        }

        swap.setLogs(swapLogs);

        return swap;
    };

    freedom::SwapProcessor* swapProcessor = m_wallet->GetSwapProcessor();

    const auto& serviceSwaps = swapProcessor->GetSwaps();

    QHash<QByteArray, Data::Swap> swaps;

    for (const auto& serviceSwap : serviceSwaps)
    {
        Data::Swap swap = mapSwap(serviceSwap);
        swaps.insert(swap.id(), swap);
    }

    Store::instance().dispatch(SwapActions::SwapsUpdated { swaps });

    connect(swapProcessor, &freedom::SwapProcessor::SwapUpdated, this, [mapSwap](const freedom::Swap& serviceSwap) {
        Data::Swap swap = mapSwap(serviceSwap);
        Store::instance().dispatch(SwapActions::SwapUpdated { swap });
    });
}

void WalletService::loadDbBalances() const
{
    const auto& accounts = m_wallet->GetAccounts();
    for (const auto& account : accounts)
    {
        freedom::core::database::FreedomDatabase   database;
        std::optional<freedom::core::api::Balance> balance_db;
        if (database.GetBalance(account->GetCurrencyUnit(), balance_db) && balance_db)
        {
            emit account->BalanceUpdated(balance_db.value());
        }
    }
}

Data::Order WalletService::mapOrder(const freedom::orderbook::order::Order& serviceOrder)
{
    const auto& assetTypeByCurrencyName = [](const QString& currencyName) -> Data::Asset::Type {
        const auto& assets = Store::instance().state().wallet().assets();

        for (const auto& asset : assets)
        {
            if (asset.currency().asset() == currencyName)
            {
                return asset.type();
            }
        }

        return Data::Asset::Type::Unknown;
    };

    freedom::core::api::IOrderBookProvider* order_book_provider = m_wallet->GetOrderClient();

    Data::Asset::Type bidAssetType = assetTypeByCurrencyName(QString::fromStdString(serviceOrder.alpha_asset.name));
    Data::Asset::Type askAssetType = assetTypeByCurrencyName(QString::fromStdString(serviceOrder.beta_asset.name));

    Data::Asset bidAsset = Store::instance().state().wallet().assets().value(bidAssetType);
    Data::Asset askAsset = Store::instance().state().wallet().assets().value(askAssetType);

    Data::Order order;

    order.setId(QByteArray::fromStdString(serviceOrder.id));
    order.setCreator(order_book_provider->IsMyOrder(serviceOrder.id) ?
                         Data::Order::Creator::Me :
                         Data::Order::Creator::Other);

    order.setBidAssetType(bidAssetType);
    order.setBidAmount(Data::Float100(serviceOrder.alpha_asset.nominal_amount));

    order.setAskAssetType(askAssetType);
    order.setAskAmount(Data::Float100(serviceOrder.beta_asset.nominal_amount));

    return order;
}

Data::Asset::Hash WalletService::mockAssets(const Data::Asset::Hash& existingAssets) const
{
    QHash<Data::Asset::Type, Data::Asset> assets = existingAssets;

    Data::Asset solana;
    solana.setType(Data::Asset::Type::Solana);
    solana.setAddress("EA6pAGp7LKA65oiJqRnuAZdsRZ7RCfPVeWF3ZYqLDLPs");
    Data::Currency solanaCurrency;
    solanaCurrency.setName("Solana");
    solanaCurrency.setCode("SOL");
    solanaCurrency.setSymbol("SOL");
    solanaCurrency.setSmallCoinsInCoin(1'000'000'000);
    solana.setCurrency(solanaCurrency);
    Data::Balance solanaBalance;
    solanaBalance.setConfirmed(0);
    solanaBalance.setUnconfirmed(0);
    solana.setBalance(solanaBalance);

    Data::Asset usdc;
    usdc.setType(Data::Asset::Type::Usdc);
    usdc.setAddress("0x68517321864C3AC67f2fe65518437F3e5b6312B5");
    Data::Currency usdcCurrency;
    usdcCurrency.setName("USD Coin");
    usdcCurrency.setCode("USDC");
    usdcCurrency.setSymbol("USDC");
    usdcCurrency.setSmallCoinsInCoin(100);
    usdc.setCurrency(usdcCurrency);
    Data::Balance usdcBalance;
    usdcBalance.setConfirmed(0);
    usdcBalance.setUnconfirmed(0);
    usdc.setBalance(usdcBalance);

    Data::Asset xrp;
    xrp.setType(Data::Asset::Type::Xrp);
    xrp.setAddress("rsutiWGxWvQimz1AZPb7Q3y1BdE1ERJWFe");
    Data::Currency xrpCurrency;
    xrpCurrency.setName("XRP");
    xrpCurrency.setCode("XRP");
    xrpCurrency.setSymbol("XRP");
    xrpCurrency.setSmallCoinsInCoin(1'000'000);
    xrp.setCurrency(xrpCurrency);
    Data::Balance xrpBalance;
    xrpBalance.setConfirmed(0);
    xrpBalance.setUnconfirmed(0);
    xrp.setBalance(xrpBalance);

    Data::Asset dogecoin;
    dogecoin.setType(Data::Asset::Type::Dogecoin);
    dogecoin.setAddress("D7vpkvB13ZPtvwv7csTBgnkT6mRDVoiUjq");
    Data::Currency dogecoinCurrency;
    dogecoinCurrency.setName("Dogecoin");
    dogecoinCurrency.setCode("DOGE");
    dogecoinCurrency.setSymbol("DOGE");
    dogecoinCurrency.setSmallCoinsInCoin(1'000'000'000);
    dogecoin.setCurrency(dogecoinCurrency);
    Data::Balance dogecoinBalance;
    dogecoinBalance.setConfirmed(0);
    dogecoinBalance.setUnconfirmed(0);
    dogecoin.setBalance(dogecoinBalance);

    Data::Asset cardano;
    cardano.setType(Data::Asset::Type::Cardano);
    cardano.setAddress("addr1qxw5n2qtjlpnlrn2qdg5tmrxv3fx204z79lkkw7l22vfytvafx5qh97r878x5q63ghkxvezjv5l29utldvaa755cjgks70zlq5");
    Data::Currency cardanoCurrency;
    cardanoCurrency.setName("Cardano");
    cardanoCurrency.setCode("ADA");
    cardanoCurrency.setSymbol("ADA");
    cardanoCurrency.setSmallCoinsInCoin(1'000'000);
    cardano.setCurrency(cardanoCurrency);
    Data::Balance cardanoBalance;
    cardanoBalance.setConfirmed(0);
    cardanoBalance.setUnconfirmed(0);
    cardano.setBalance(cardanoBalance);

    Data::Asset polkadot;
    polkadot.setType(Data::Asset::Type::Polkadot);
    polkadot.setAddress("1T1hr9NeRvhMC4cNaBaRN1TknyUaCb7fnipnHz669kFQC1W");
    Data::Currency polkadotCurrency;
    polkadotCurrency.setName("Polkadot");
    polkadotCurrency.setCode("DOT");
    polkadotCurrency.setSymbol("DOT");
    polkadotCurrency.setSmallCoinsInCoin(1'000'000);
    polkadot.setCurrency(polkadotCurrency);
    Data::Balance polkadotBalance;
    polkadotBalance.setConfirmed(0);
    polkadotBalance.setUnconfirmed(0);
    polkadot.setBalance(polkadotBalance);

    assets.insert({
        {Data::Asset::Type::Solana,    solana  },
        { Data::Asset::Type::Usdc,     usdc    },
        { Data::Asset::Type::Xrp,      xrp     },
        { Data::Asset::Type::Dogecoin, dogecoin},
        { Data::Asset::Type::Cardano,  cardano },
        { Data::Asset::Type::Polkadot, polkadot},
    });

    return assets;
}

} // namespace XWallet
