#pragma once

#include "NumericTypes.h"

#include <QSharedDataPointer>
#include <QtQmlIntegration>

namespace Data {

class CurrencyData;

class Currency final
{
    Q_GADGET
    QML_VALUE_TYPE(currency)

    Q_PROPERTY(QString name READ name CONSTANT FINAL)
    Q_PROPERTY(QString asset READ asset CONSTANT FINAL)
    Q_PROPERTY(QString symbol READ symbol CONSTANT FINAL)
    Q_PROPERTY(QString code READ code CONSTANT FINAL)
    Q_PROPERTY(quint32 decimal READ numberOfDecimal CONSTANT FINAL)

public:
    explicit Currency();

    Currency(Currency&& other) noexcept;
    Currency(const Currency& other) noexcept;
    ~Currency();

    Currency& operator= (const Currency& other) noexcept;
    bool      operator== (const Currency& other) const;

    QString name() const;
    void    setName(const QString& name);

    QString asset() const;
    void    setAsset(const QString& asset);

    QString symbol() const;
    void    setSymbol(const QString& symbol);

    QString code() const;
    void    setCode(const QString& code);

    quint32 numberOfDecimal() const;
    void    setNumberOfDecimal(quint32 numberOfDecimals);

    UInt256 smallCoinsInCoin() const;
    void    setSmallCoinsInCoin(const UInt256& smallCoinsInCoin);

private:
    QSharedDataPointer<CurrencyData> data;
};

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Currency, Q_MOVABLE_TYPE);
