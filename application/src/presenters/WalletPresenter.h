#pragma once

#include "data/Wallet.h"

namespace XWallet::Presenters {

class WalletPresenter : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Data::Wallet wallet READ wallet WRITE setWallet NOTIFY walletChanged REQUIRED FINAL)

    Q_PROPERTY(QString balance READ balance NOTIFY balanceChanged FINAL)
    Q_PROPERTY(QString fiatBalance READ fiatBalance NOTIFY fiatBalanceChanged FINAL)

public:
    WalletPresenter(QObject* parent = nullptr);

private:
    Data::Wallet wallet() const { return m_wallet; }
    void         setWallet(const Data::Wallet& wallet);

    QString balance() const { return m_balance; }
    void    setBalance(const QString& balance);

    QString fiatBalance() const { return m_fiatBalance; }
    void    setFiatBalance(const QString& fiatBalance);

signals:
    void walletChanged();

    void balanceChanged();
    void fiatBalanceChanged();

private:
    Data::Wallet m_wallet;

    QString m_balance;
    QString m_fiatBalance;
};

} // namespace XWallet::Presenters
