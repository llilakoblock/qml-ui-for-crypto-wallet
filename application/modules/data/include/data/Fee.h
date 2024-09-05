#pragma once

#include "NumericTypes.h"

#include <QSharedDataPointer>
#include <QtQmlIntegration>

namespace Data {

class FeeData;

class Fee final
{
    Q_GADGET
    QML_VALUE_TYPE(fee)

public:
    explicit Fee();

    Fee(Fee&& other) noexcept;
    Fee(const Fee& other) noexcept;
    ~Fee();

    Fee& operator= (const Fee& other) noexcept;
    bool operator== (const Fee& other) const;

    UInt256 estimateFee() const;
    void    setEstimateFee(const UInt256& estimateFee);

    UInt256 fundFee() const;
    void    setFundFee(const UInt256& fundFee);

    UInt256 refundFee() const;
    void    setRefundFee(const UInt256& refundFee);

    UInt256 redeemFee() const;
    void    setRedeemFee(const UInt256& redeemFee);

    UInt256 standardTxFee() const;
    void    setStandardTxFee(const UInt256& standardTxFee);

private:
    QSharedDataPointer<FeeData> data;
};

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Fee, Q_MOVABLE_TYPE);
