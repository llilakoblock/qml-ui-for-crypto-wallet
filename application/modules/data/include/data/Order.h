#pragma once

#include "data/Asset.h"

#include <QQmlEngine>
#include <QSharedDataPointer>

namespace Data {

class OrderData;

class Order
{
    Q_GADGET
    QML_VALUE_TYPE(order)

    Q_PROPERTY(QByteArray id READ id CONSTANT FINAL)

    Q_PROPERTY(Creator creator READ creator CONSTANT FINAL)

    Q_PROPERTY(Asset::Type bidAssetType READ bidAssetType FINAL)
    Q_PROPERTY(Asset::Type askAssetType READ askAssetType FINAL)

public:
    enum class Creator
    {
        Unknown,
        Me,
        Other,
    };
    Q_ENUM(Creator)

public:
    explicit Order();

    Order(Order&& other) noexcept;
    Order(const Order& other) noexcept;
    ~Order();

    Order& operator= (const Order& other) noexcept;
    bool   operator== (const Order& other) const;

    QByteArray id() const;
    void       setId(const QByteArray& id);

    Creator creator() const;
    void    setCreator(Creator creator);

    Asset::Type bidAssetType() const;
    void        setBidAssetType(Asset::Type bidAssetType);

    Float100 bidAmount() const;
    void     setBidAmount(const Float100& bidAmountType);

    Asset::Type askAssetType() const;
    void        setAskAssetType(Asset::Type askAssetType);

    Float100 askAmount() const;
    void     setAskAmount(const Float100& askAmount);

private:
    QSharedDataPointer<OrderData> data;
};

class OrderDerived : public Order
{
    Q_GADGET
};

namespace OrderDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(Order)
QML_FOREIGN_NAMESPACE(Data::OrderDerived)
}; // namespace OrderDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Order, Q_MOVABLE_TYPE);
