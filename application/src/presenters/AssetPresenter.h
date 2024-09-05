#pragma once

#include "data/Asset.h"

namespace XWallet::Presenters {

class AssetPresenter : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Data::Asset asset READ asset WRITE setAsset NOTIFY assetChanged REQUIRED FINAL)

    Q_PROPERTY(Data::Asset parentAsset READ parentAsset NOTIFY parentAssetChanged FINAL)
    Q_PROPERTY(bool parentAssetExists READ parentAssetExists NOTIFY parentAssetExistsChanged FINAL)

    Q_PROPERTY(QString balance READ balance NOTIFY balanceChanged FINAL)
    Q_PROPERTY(QString fiatBalance READ fiatBalance NOTIFY fiatBalanceChanged FINAL)
    Q_PROPERTY(QString fiatPrice READ fiatPrice NOTIFY fiatPriceChanged FINAL)

    Q_PROPERTY(QString fee READ fee NOTIFY feeChanged FINAL)
    Q_PROPERTY(QString fiatFee READ fiatFee NOTIFY fiatFeeChanged FINAL)

public:
    AssetPresenter(QObject* parent = nullptr);

private:
    Data::Asset asset() const { return m_asset; }
    void        setAsset(const Data::Asset& asset);

    Data::Asset parentAsset() const { return m_parentAsset; }
    void        setParentAsset(const Data::Asset& parentAsset);

    bool parentAssetExists() const { return m_parentAssetExists; }
    void setParentAssetExists(bool parentAssetExists);

    QString balance() const { return m_balance; }
    void    setBalance(const QString& balance);

    QString fiatBalance() const { return m_fiatBalance; }
    void    setFiatBalance(const QString& fiatBalance);

    QString fiatPrice() const { return m_fiatPrice; }
    void    setFiatPrice(const QString& fiatPrice);

    QString fee() const { return m_fee; }
    void    setFee(const QString& fee);

    QString fiatFee() const { return m_fiatFee; }
    void    setFiatFee(const QString& fiatFee);

signals:
    void assetChanged();

    void parentAssetChanged();

    void balanceChanged();
    void fiatBalanceChanged();
    void fiatPriceChanged();

    void feeChanged();
    void fiatFeeChanged();

    void parentAssetExistsChanged();

private:
    Data::Asset m_asset;
    Data::Asset m_parentAsset;

    bool m_parentAssetExists { false };

    QString m_balance;
    QString m_fiatBalance;
    QString m_fiatPrice;
    QString m_fee;
    QString m_fiatFee;
};

} // namespace XWallet::Presenters
