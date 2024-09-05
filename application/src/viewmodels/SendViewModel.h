#pragma once

#include "data/Asset.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class SendViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(Errors errors READ errors NOTIFY errorsChanged FINAL)

    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)

    Q_PROPERTY(QString remainingBalance READ remainingBalance NOTIFY remainingBalanceChanged FINAL)

    Q_PROPERTY(Data::HistoryEntry transaction READ transaction NOTIFY transactionChanged FINAL)

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)

    Q_PROPERTY(QString coinsToSend READ coinsToSend WRITE setCoinsToSend NOTIFY coinsToSendChanged FINAL)
    Q_PROPERTY(QString fiatsToSend READ fiatsToSend WRITE setFiatsToSend NOTIFY fiatsToSendChanged FINAL)

    Q_PROPERTY(Data::Asset asset READ asset WRITE setAsset NOTIFY assetChanged REQUIRED FINAL)

public:
    enum class Status
    {
        UnknownStatus,
        BusyStatus,
        SuccessStatus,
        ErrorStatus,
    };
    Q_ENUM(Status)

    enum class ChangeReason
    {
        Unknown,
        UserInput,
        Calculations,
        StateChanged,
    };
    Q_ENUM(ChangeReason)

    enum class Error
    {
        NoError                 = 0,
        ZeroAmountError         = 1 << 0,
        InsufficientAmountError = 1 << 1,
        WrongFeeAmountError     = 1 << 2,
        WrongAddressError       = 1 << 3,
    };
    Q_DECLARE_FLAGS(Errors, Error)
    Q_FLAG(Error)

    enum class LastField
    {
        UnknownField,
        CoinsField,
        FiatsField,
    };
    Q_ENUM(LastField)

public:
    explicit SendViewModel(QObject* parent = nullptr);

    Q_INVOKABLE Errors makeTransaction();
    Q_INVOKABLE void   leaveNote(const QString& note);

    Q_INVOKABLE void reset();

private:
    static QString prepareUserInput(const QString& userInput);

    Status status() const { return m_status; }
    void   setStatus(Status status);

    Errors errors() const { return m_errors; }
    void   setErrors(Errors errors);

    QString errorMessage() const { return m_errorMessage; }
    void    setErrorMessage(const QString& errorMessage);

    QString remainingBalance() const { return m_remainingBalance; }
    void    setRemainingBalance(const QString& remainingBalance);

    Data::HistoryEntry transaction() const { return m_transaction; }
    void               setTransaction(const Data::HistoryEntry& transaction);

    QString address() const { return m_address; }
    void    setAddress(const QString& address);

    QString coinsToSend() const { return m_coinsToSend; }
    void    setCoinsToSend(const QString& coinsToSend, ChangeReason changeReason = ChangeReason::UserInput);

    QString fiatsToSend() const { return m_fiatsToSend; }
    void    setFiatsToSend(const QString& fiatsToSend, ChangeReason changeReason = ChangeReason::UserInput);

    Data::Asset asset() const { return m_asset; }
    void        setAsset(const Data::Asset& asset);

signals:
    void statusChanged();
    void errorsChanged();

    void remainingBalanceChanged();

    void transactionChanged();

    void coinsToSendChanged(const QString& coinsToSend, ChangeReason changeReason);
    void fiatsToSendChanged(const QString& fiatsToSend, ChangeReason changeReason);

    void assetChanged();

    void addressChanged();

    void errorMessageChanged();

private:
    Status m_status { Status::UnknownStatus };
    Errors m_errors { Error::NoError };

    QString m_errorMessage;

    QString m_address;

    QString m_coinsToSend;
    QString m_fiatsToSend;

    QString m_remainingBalance;

    Data::Asset m_asset;

    Data::HistoryEntry m_transaction;

    QString m_awaitingTxId;

    LastField m_lastField { LastField::UnknownField };
};

} // namespace XWallet::ViewModels
