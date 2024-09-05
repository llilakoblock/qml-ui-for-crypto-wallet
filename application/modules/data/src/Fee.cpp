#include "data/Fee.h"

#include "FeeData.h"

namespace Data {

Fee::Fee()
    : data(new FeeData)
{}

Fee::Fee(Fee&& other) noexcept      = default;
Fee::Fee(const Fee& other) noexcept = default;
Fee::~Fee()                         = default;

Fee& Fee::operator= (const Fee& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Fee::operator== (const Fee& other) const
{
    return data == other.data || *data == *other.data;
}

UInt256 Fee::estimateFee() const
{
    return data->estimateFee;
}

void Fee::setEstimateFee(const UInt256& estimateFee)
{
    data->estimateFee = estimateFee;
}

UInt256 Fee::fundFee() const
{
    return data->fundFee;
}

void Fee::setFundFee(const UInt256& fundFee)
{
    data->fundFee = fundFee;
}

UInt256 Fee::refundFee() const
{
    return data->refundFee;
}

void Fee::setRefundFee(const UInt256& refundFee)
{
    data->refundFee = refundFee;
}

UInt256 Fee::redeemFee() const
{
    return data->redeemFee;
}

void Fee::setRedeemFee(const UInt256& redeemFee)
{
    data->redeemFee = redeemFee;
}

UInt256 Fee::standardTxFee() const
{
    return data->standardTxFee;
}

void Fee::setStandardTxFee(const UInt256& standardTxFee)
{
    data->standardTxFee = standardTxFee;
}

} // namespace Data
