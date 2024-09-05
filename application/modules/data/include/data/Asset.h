#pragma once

#include "Balance.h"
#include "Currency.h"
#include "Fee.h"
#include "HistoryEntry.h"
#include "NumericTypes.h"
#include "Quote.h"

#include <QQmlEngine>
#include <QSharedDataPointer>

namespace Data {

class AssetData;

class Asset
{
    Q_GADGET
    QML_VALUE_TYPE(asset)

    Q_PROPERTY(Type type READ type CONSTANT FINAL)
    Q_PROPERTY(Type parentType READ parentType CONSTANT FINAL)
    Q_PROPERTY(QString address READ address CONSTANT FINAL)
    Q_PROPERTY(Currency currency READ currency CONSTANT FINAL)
    Q_PROPERTY(Balance balance READ balance CONSTANT FINAL)
    Q_PROPERTY(Fee fee READ fee CONSTANT FINAL)
    Q_PROPERTY(Quote quote READ quote READ quote CONSTANT FINAL)
    Q_PROPERTY(QList<HistoryEntry> history READ history CONSTANT FINAL)

    Q_PROPERTY(bool testnet READ isTestnet CONSTANT FINAL)

    Q_PROPERTY(qreal difference READ difference CONSTANT FINAL)

public:
    enum class Blockchain
    {
        UnknownBlockchain,
        BitcoinBlockchain,
        EthereumBlockchain
    };
    Q_ENUM(Blockchain)

    enum class Type
    {
        Unknown,
        Bitcoin,
        Ethereum,
        Tether,
        VaraNetwork,
        // MOCK
        Solana,
        Usdc,
        Xrp,
        Dogecoin,
        Cardano,
        Polkadot,
        // MOCK

        // Top 10 cryptocurrencies
        // BTC (bitcoin)
        // ETH (ethereum)
        // USDT (tether)
        // SOL (solana)
        // USDC (usdc)
        // XRP (xrp)
        // DOGE (dogecoin)
        // ADA (cardano)
        // DOT (polkadot)
        // VARA (vara network)
    };
    Q_ENUM(Type)

    enum class Resolution
    {
        Unknown,
        Max,
        Mid,
        Min
    };
    Q_ENUM(Resolution)

    using Hash = QHash<Type, Asset>;

public:
    explicit Asset();

    Asset(Asset&& other) noexcept;
    Asset(const Asset& other) noexcept;
    ~Asset();

    Asset& operator= (const Asset& other) noexcept;
    bool   operator== (const Asset& other) const;

    Q_INVOKABLE QString fiatAmountFromCoin(const QString& coinAmount) const;

    Type type() const;
    void setType(Type type);

    Type parentType() const;
    void setParentType(Type parentType);

    QString address() const;
    void    setAddress(const QString& address);

    Currency currency() const;
    void     setCurrency(const Currency& currency);

    Balance balance() const;
    void    setBalance(const Balance& balance);

    Fee  fee() const;
    void setFee(const Fee& fee);

    Quote quote() const;
    void  setQuote(const Quote& quote);

    QList<HistoryEntry> history() const;
    void                setHistory(const QList<HistoryEntry>& history);

    QList<Quote> quotes(Resolution resolution) const;
    void         setQuotes(const QList<Quote>& quotes, Resolution resolution);

    bool isTestnet() const;
    void setTestnet(bool isTestnet);

    qreal difference() const;

    Float100 smallCoinToBigCoinValue(const UInt256& value) const;
    Float100 smallCoinToBigCoinValue(const Int256& value) const;

private:
    QSharedDataPointer<AssetData> data;
};

class AssetDerived : public Asset
{
    Q_GADGET
};

namespace AssetDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(Asset)
QML_FOREIGN_NAMESPACE(Data::AssetDerived)
}; // namespace AssetDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Asset, Q_MOVABLE_TYPE);
