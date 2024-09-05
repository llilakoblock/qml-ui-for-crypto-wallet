#pragma once

#include "NumericTypes.h"

#include <QSharedDataPointer>
#include <QtQmlIntegration>

namespace Data {

class BalanceData;

class Balance final
{
    Q_GADGET
    QML_VALUE_TYPE(balance)

public:
    explicit Balance();

    Balance(Balance&& other) noexcept;
    Balance(const Balance& other) noexcept;
    ~Balance();

    Balance& operator= (const Balance& other) noexcept;
    bool     operator== (const Balance& other) const;
    bool     operator< (const Balance& other) const;
    bool     operator> (const Balance& other) const;

    Int256 total() const;

    UInt256 confirmed() const;
    void    setConfirmed(const UInt256& confirmed);

    Int256 unconfirmed() const;
    void   setUnconfirmed(const Int256& unconfirmed);

private:
    QSharedDataPointer<BalanceData> data;
};

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Balance, Q_MOVABLE_TYPE);
