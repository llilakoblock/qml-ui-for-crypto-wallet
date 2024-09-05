#include "WalletPresenter.h"

#include "singletones/Store.h"

namespace XWallet::Presenters {

WalletPresenter::WalletPresenter(QObject* parent)
    : QObject(parent)
{
    connect(this, &WalletPresenter::walletChanged, this, [this]() {
        const auto& assets = Store::instance().state().wallet().assets();

        Data::Float100 balance { 0 };
        Data::Float100 fiatBalance { 0 };

        for (const auto& asset : assets)
        {
            balance += asset.smallCoinToBigCoinValue(asset.balance().total());
            fiatBalance += asset.smallCoinToBigCoinValue(asset.balance().total()) * asset.quote().price();
        }

        QString balanceString     = Data::toPrettyString(balance);
        QString fiatBalanceString = Data::toPrettyString(fiatBalance, 2, 2);

        setBalance(balanceString);
        setFiatBalance(fiatBalanceString);
    });
}

void WalletPresenter::setWallet(const Data::Wallet& wallet)
{
    if (m_wallet == wallet)
    {
        return;
    }
    m_wallet = wallet;
    emit walletChanged();
}

void WalletPresenter::setBalance(const QString& balance)
{
    if (m_balance == balance)
    {
        return;
    }
    m_balance = balance;
    emit balanceChanged();
}

void WalletPresenter::setFiatBalance(const QString& fiatBalance)
{
    if (m_fiatBalance == fiatBalance)
    {
        return;
    }
    m_fiatBalance = fiatBalance;
    emit fiatBalanceChanged();
}

} // namespace XWallet::Presenters
