#include "AssetPresenter.h"

#include "singletones/Store.h"

namespace XWallet::Presenters {

AssetPresenter::AssetPresenter(QObject* parent)
    : QObject(parent)
{
    connect(this, &AssetPresenter::assetChanged, this, [this]() {
        bool parentAssetExists = (m_asset.parentType() != Data::Asset::Type::Unknown);

        QString balance     = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.balance().total()));
        QString fiatBalance = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.balance().total()) * m_asset.quote().price(), 2, 2);
        QString fiatPrice   = Data::toPrettyString(m_asset.quote().price(), 2, 2);

        QString fee     = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()));
        QString fiatFee = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()) * m_asset.quote().price(), 2, 2);

        if (parentAssetExists && Store::instance().state().wallet().assets().contains(m_asset.parentType()))
        {
            const auto& parentAsset = Store::instance().state().wallet().assets().value(m_asset.parentType());

            fee     = Data::toPrettyString(parentAsset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()));
            fiatFee = Data::toPrettyString(parentAsset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()) * parentAsset.quote().price(), 2, 2);

            setParentAsset(parentAsset);
            setParentAssetExists(true);
        }
        else
        {
            fee     = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()));
            fiatFee = Data::toPrettyString(m_asset.smallCoinToBigCoinValue(m_asset.fee().standardTxFee()) * m_asset.quote().price(), 2, 2);

            setParentAssetExists(false);
        }

        if (fiatFee == "0.00"_L1)
        {
            fiatFee = "<0.01"_L1;
        }

        setBalance(balance);
        setFiatBalance(fiatBalance);
        setFiatPrice(fiatPrice);
        setFee(fee);
        setFiatFee(fiatFee);
    });
}

void AssetPresenter::setAsset(const Data::Asset& asset)
{
    if (m_asset == asset)
    {
        return;
    }
    m_asset = asset;
    emit assetChanged();
}

void AssetPresenter::setParentAsset(const Data::Asset& parentAsset)
{
    if (m_parentAsset == parentAsset)
    {
        return;
    }
    m_parentAsset = parentAsset;
    emit parentAssetChanged();
}

void AssetPresenter::setParentAssetExists(bool parentAssetExists)
{
    if (m_parentAssetExists == parentAssetExists)
    {
        return;
    }
    m_parentAssetExists = parentAssetExists;
    emit parentAssetExistsChanged();
}

void AssetPresenter::setBalance(const QString& balance)
{
    if (m_balance == balance)
    {
        return;
    }
    m_balance = balance;
    emit balanceChanged();
}

void AssetPresenter::setFiatBalance(const QString& fiatBalance)
{
    if (m_fiatBalance == fiatBalance)
    {
        return;
    }
    m_fiatBalance = fiatBalance;
    emit fiatBalanceChanged();
}

void AssetPresenter::setFiatPrice(const QString& fiatPrice)
{
    if (m_fiatPrice == fiatPrice)
    {
        return;
    }
    m_fiatPrice = fiatPrice;
    emit fiatPriceChanged();
}

void AssetPresenter::setFee(const QString& fee)
{
    if (m_fee == fee)
    {
        return;
    }
    m_fee = fee;
    emit feeChanged();
}

void AssetPresenter::setFiatFee(const QString& fiatFee)
{
    if (m_fiatFee == fiatFee)
    {
        return;
    }
    m_fiatFee = fiatFee;
    emit fiatFeeChanged();
}

} // namespace XWallet::Presenters
