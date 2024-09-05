#include "SendViewModel.h"

#include "singletones/Store.h"
#include "data/NumericTypes.h"
#include "middleware/WalletMiddleware.h"

namespace XWallet::ViewModels {

SendViewModel::SendViewModel(QObject* parent)
    : QObject(parent)
{
    connect(this, &SendViewModel::coinsToSendChanged, this, [this](const QString& coinsInput, ChangeReason changeReason) {
        if (changeReason == ChangeReason::Calculations)
        {
            return;
        }

        const auto& coinsValue = prepareUserInput(coinsInput);

        Data::Float100 availableCoins = m_asset.smallCoinToBigCoinValue(m_asset.balance().total());
        Data::Float100 availableFiat  = availableCoins * m_asset.quote().price();

        if (coinsValue.isEmpty())
        {
            setFiatsToSend(QString(), ChangeReason::Calculations);
            setErrors(errors().setFlag(Error::ZeroAmountError));

            setRemainingBalance(Data::toPrettyString(availableCoins));

            return;
        }

        Data::Float100 coinsToSend = Data::Float100(coinsValue.toStdString());
        Data::Float100 fiatToSend  = coinsToSend * m_asset.quote().price();

        setErrors(errors().setFlag(Error::InsufficientAmountError, fiatToSend > availableFiat));
        setErrors(errors().setFlag(Error::ZeroAmountError, coinsToSend.is_zero()));

        setFiatsToSend(Data::toPrettyString(fiatToSend, 2, 2), ChangeReason::Calculations);

        Data::Float100 remainingBalance = availableCoins - coinsToSend;

        setRemainingBalance(Data::toPrettyString(remainingBalance));
    });

    connect(this, &SendViewModel::fiatsToSendChanged, this, [this](const QString& fiatInput, ChangeReason changeReason) {
        if (changeReason == ChangeReason::Calculations)
        {
            return;
        }

        const auto& fiatsValue = prepareUserInput(fiatInput);

        Data::Float100 availableCoins = m_asset.smallCoinToBigCoinValue(m_asset.balance().total());

        if (fiatsValue.isEmpty())
        {
            setCoinsToSend(QString(), ChangeReason::Calculations);
            setErrors(errors().setFlag(Error::ZeroAmountError));

            setRemainingBalance(Data::toPrettyString(availableCoins));

            return;
        }

        Data::Float100 coinsToSend = Data::Float100(fiatsValue.toStdString()) / m_asset.quote().price();

        setErrors(errors().setFlag(Error::InsufficientAmountError, coinsToSend > availableCoins));
        setErrors(errors().setFlag(Error::ZeroAmountError, coinsToSend.is_zero()));

        setCoinsToSend(Data::toPrettyString(coinsToSend), ChangeReason::Calculations);

        Data::Float100 remainingBalance = availableCoins - coinsToSend;

        setRemainingBalance(Data::toPrettyString(remainingBalance));
    });

    connect(this, &SendViewModel::assetChanged, this, [this]() {
        switch (m_lastField)
        {
            case LastField::CoinsField:
                setCoinsToSend(m_coinsToSend, ChangeReason::StateChanged);
                break;
            case LastField::FiatsField:
                setFiatsToSend(m_fiatsToSend, ChangeReason::StateChanged);
                break;
            default:
                Data::Float100 availableCoins = m_asset.smallCoinToBigCoinValue(m_asset.balance().total());
                setRemainingBalance(Data::toPrettyString(availableCoins));
                break;
        }
    });

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        if (!state.wallet().assets().contains(m_asset.type()))
        {
            return;
        }

        const auto& asset = state.wallet().assets().value(m_asset.type());

        setAsset(asset);

        if (m_awaitingTxId.isEmpty())
        {
            return;
        }

        const auto& historyEntryes = asset.history();

        if (historyEntryes.isEmpty())
        {
            return;
        }

        for (const Data::HistoryEntry& entry : historyEntryes)
        {
            if (entry.hash() == m_awaitingTxId)
            {
                setTransaction(entry);
                return;
            }
        }
    });

    connect(Store::instance().walletMiddleware().data(), &Middleware::WalletMiddleware::transactionCompleted, this, [this](const WalletServiceActions::MakeTransaction& action, const QString& txId) {
        m_awaitingTxId = txId;

        setStatus(Status::SuccessStatus);
        setErrorMessage(QString());
    });

    connect(Store::instance().walletMiddleware().data(), &Middleware::WalletMiddleware::transactionFailed, this, [this](const WalletServiceActions::MakeTransaction& action, const QString& error) {
        qWarning() << Q_FUNC_INFO << "Transaction failed with error:" << error;

        setStatus(Status::ErrorStatus);
        setErrorMessage(error);
    });
}

SendViewModel::Errors SendViewModel::makeTransaction()
{
    const auto& coinsValue = prepareUserInput(m_coinsToSend);

    if (coinsValue.isEmpty())
    {
        setErrors(errors().setFlag(Error::ZeroAmountError));
        return errors();
    }

    Data::Float100 availableCoins = m_asset.smallCoinToBigCoinValue(m_asset.balance().total());
    Data::Float100 coinsToSend    = Data::Float100(coinsValue.toStdString());
    Data::Float100 fiatToSend     = coinsToSend * m_asset.quote().price();

    setErrors(errors().setFlag(Error::InsufficientAmountError, coinsToSend > availableCoins));
    setErrors(errors().setFlag(Error::ZeroAmountError, coinsToSend.is_zero()));

    setCoinsToSend(Data::toPrettyString(coinsToSend), ChangeReason::Calculations);
    setFiatsToSend(Data::toPrettyString(fiatToSend, 2, 2), ChangeReason::Calculations);

    if (errors())
    {
        return errors();
    }

    Store::instance().dispatch(WalletServiceActions::MakeTransaction { m_asset.type(), m_address, prepareUserInput(m_coinsToSend), QString() });

    setStatus(Status::BusyStatus);

    return errors();
}

void SendViewModel::leaveNote(const QString& note)
{
    QList<Data::HistoryEntry> history = m_asset.history();

    if (history.isEmpty())
    {
        return;
    }

    for (qsizetype i = 0; i < history.size(); i++)
    {
        if (history.at(i).hash() == m_awaitingTxId)
        {
            auto newHistoryEntry = history.at(i);
            newHistoryEntry.setNote(note);

            history.replace(i, newHistoryEntry);
            break;
        }
    }

    Store::instance().dispatch(DatabaseServiceActions::StoreHistoryNotes { m_asset.type(), history });

    reset();
}

void SendViewModel::reset()
{
    setStatus(Status::UnknownStatus);
    setErrors(Error::NoError);

    setErrorMessage(QString());

    setTransaction(Data::HistoryEntry());

    setCoinsToSend(QString(), ChangeReason::StateChanged);
    setFiatsToSend(QString(), ChangeReason::StateChanged);

    m_awaitingTxId.clear();
}

QString SendViewModel::prepareUserInput(const QString& userInput)
{
    if (userInput.isEmpty())
    {
        return userInput;
    }

    QString valueWithoutCommas = userInput;
    valueWithoutCommas.remove(',');

    if (valueWithoutCommas == '.')
    {
        return QStringLiteral("0");
    }
    else if (valueWithoutCommas.last(1) == '.')
    {
        return valueWithoutCommas.chopped(1);
    }
    else if (valueWithoutCommas.first(1) == '.')
    {
        return QStringLiteral("0%1").arg(valueWithoutCommas);
    }

    return valueWithoutCommas;
}

void SendViewModel::setStatus(Status status)
{
    if (m_status == status)
    {
        return;
    }
    m_status = status;
    emit statusChanged();
}

void SendViewModel::setErrors(Errors errors)
{
    if (m_errors == errors)
    {
        return;
    }
    m_errors = errors;
    emit errorsChanged();
}

void SendViewModel::setErrorMessage(const QString& errorMessage)
{
    if (m_errorMessage == errorMessage)
    {
        return;
    }
    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}

void SendViewModel::setRemainingBalance(const QString& remainingBalance)
{
    if (m_remainingBalance == remainingBalance)
    {
        return;
    }
    m_remainingBalance = remainingBalance;
    emit remainingBalanceChanged();
}

void SendViewModel::setTransaction(const Data::HistoryEntry& transaction)
{
    if (m_transaction == transaction)
    {
        return;
    }
    m_transaction = transaction;
    emit transactionChanged();
}

void SendViewModel::setAddress(const QString& address)
{
    if (m_address == address)
    {
        return;
    }
    m_address = address;
    emit addressChanged();
}

void SendViewModel::setCoinsToSend(const QString& coinsToSend, ChangeReason changeReason)
{
    if (m_coinsToSend == coinsToSend)
    {
        return;
    }
    m_coinsToSend = coinsToSend;
    emit coinsToSendChanged(coinsToSend, changeReason);

    if (changeReason == ChangeReason::UserInput)
    {
        m_lastField = LastField::CoinsField;
    }
}

void SendViewModel::setFiatsToSend(const QString& fiatsToSend, ChangeReason changeReason)
{
    if (m_fiatsToSend == fiatsToSend)
    {
        return;
    }
    m_fiatsToSend = fiatsToSend;
    emit fiatsToSendChanged(fiatsToSend, changeReason);

    if (changeReason == ChangeReason::UserInput)
    {
        m_lastField = LastField::FiatsField;
    }
}

void SendViewModel::setAsset(const Data::Asset& asset)
{
    if (m_asset == asset)
    {
        return;
    }
    m_asset = asset;
    emit assetChanged();
}

} // namespace XWallet::ViewModels
