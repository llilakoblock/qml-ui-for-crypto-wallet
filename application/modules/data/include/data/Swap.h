#pragma once

#include "data/Order.h"
#include "data/SwapLog.h"

#include <QQmlEngine>
#include <QSharedDataPointer>
#include <QStack>

namespace Data {

class SwapData;

class Swap
{
    Q_GADGET
    QML_VALUE_TYPE(swap)

    Q_PROPERTY(QByteArray id READ id CONSTANT FINAL)

    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT FINAL)

    Q_PROPERTY(Order myOrder READ myOrder CONSTANT FINAL)
    Q_PROPERTY(Order otherOrder READ otherOrder CONSTANT FINAL)

    Q_PROPERTY(Role role READ role CONSTANT FINAL)
    Q_PROPERTY(State state READ state CONSTANT FINAL)

    Q_PROPERTY(QList<SwapLog> logs READ logs CONSTANT FINAL)

public:
    enum class Role
    {
        UnknownRole,
        AliceRole,
        BobRole
    };
    Q_ENUM(Role)

    enum class State
    {
        EmptyState,
        ReadyForFundState,
        FundingState,
        FundFailedState,
        FundSucceedState,
        ReadyForRedeemState,
        RedeemingState,
        RedeemFailedState,
        RedeemSucceedState,
        ReadyForRefundState,
        RefundingState,
        RefundFailedState,
        RefundSucceedState,
        FinishedState,
        DeclinedState,
        ErrorState,
    };
    Q_ENUM(State)

public:
    explicit Swap();

    Swap(Swap&& other) noexcept;
    Swap(const Swap& other) noexcept;
    ~Swap();

    Swap& operator= (const Swap& other) noexcept;
    bool  operator== (const Swap& other) const;

    QByteArray id() const;
    void       setId(const QByteArray& id);

    QDateTime timestamp() const;
    void      setTimestamp(const QDateTime& timestamp);

    Order myOrder() const;
    void  setMyOrder(const Order& order);

    Order otherOrder() const;
    void  setOtherOrder(const Order& order);

    Role role() const;
    void setRole(Role role);

    State state() const;
    void  setState(State state);

    QList<SwapLog> logs() const;
    void           setLogs(const QList<SwapLog>& logs);

private:
    QSharedDataPointer<SwapData> data;
};

class SwapDerived : public Swap
{
    Q_GADGET
};

namespace SwapDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(Swap)
QML_FOREIGN_NAMESPACE(Data::SwapDerived)
}; // namespace SwapDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Swap, Q_MOVABLE_TYPE);
