#pragma once

#include "data/Asset.h"
#include "data/Order.h"
#include "data/Swap.h"

#include <QQmlEngine>
#include <QSharedDataPointer>

namespace Data {

class WalletData;

class Wallet
{
    Q_GADGET
    QML_VALUE_TYPE(wallet)

    Q_PROPERTY(State state READ state CONSTANT FINAL)

public:
    enum class State
    {
        UnknownState,
        UnexistState,
        LockedState,
        UnlockedState,
    };
    Q_ENUM(State)

    enum class Error
    {
        None = 0,
    };
    Q_DECLARE_FLAGS(Errors, Error)
    Q_FLAG(Error)

public:
    explicit Wallet();

    Wallet(Wallet&& other) noexcept;
    Wallet(const Wallet& other) noexcept;
    ~Wallet();

    Wallet& operator= (const Wallet& other) noexcept;
    bool    operator== (const Wallet& other) const;

    State state() const;
    void  setState(State state);

    Asset::Hash assets() const;
    void        setAssets(const Asset::Hash& assets);

    QHash<QByteArray, Order> orders() const;
    void                     setOrders(const QHash<QByteArray, Order>& orders);

    QHash<QByteArray, Swap> swaps() const;
    void                    setSwaps(const QHash<QByteArray, Swap>& swaps);

private:
    QSharedDataPointer<WalletData> data;
};

class WalletDerived : public Wallet
{
    Q_GADGET
};

namespace WalletDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(Wallet)
QML_FOREIGN_NAMESPACE(Data::WalletDerived)
}; // namespace WalletDerivedForeign

} // namespace Data

Q_DECLARE_OPERATORS_FOR_FLAGS(Data::Wallet::Errors)
Q_DECLARE_TYPEINFO(Data::Wallet, Q_MOVABLE_TYPE);
