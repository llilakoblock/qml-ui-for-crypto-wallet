#include "SwapViewModel.h"

#include "singletones/Store.h"
#include "middleware/WalletMiddleware.h"

namespace XWallet::ViewModels {

SwapViewModel::SwapViewModel(QObject* parent)
    : QObject(parent)
    , m_assetsModel(new Models::AssetsModel)
{
    const auto& swapAssets = QList<Data::Asset::Type> {
        Data::Asset::Type::Bitcoin,
        Data::Asset::Type::Ethereum,
        Data::Asset::Type::Tether,
        Data::Asset::Type::VaraNetwork,
    };

    m_sortedAssetsModel.reset(new SortFilterProxyModelByCondition(m_assetsModel.data(), [this, swapAssets](const QModelIndex& index) -> bool {
        const auto& asset = index.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

        if (!swapAssets.contains(asset.type()))
        {
            return false;
        }

        if (m_searchQuery.isEmpty() && m_filterAsset == FilterAsset::UnknownAsset)
        {
            return true;
        }

        switch (m_filterAsset)
        {
            case FilterAsset::BidAsset:
                if (asset.type() == m_bidAsset.type())
                {
                    return false;
                }
                break;
            case FilterAsset::AskAsset:
                if (asset.type() == m_askAsset.type())
                {
                    return false;
                }
                break;
            default:
                break;
        }

        if (m_searchQuery.isEmpty())
        {
            return true;
        }

        bool result = asset.currency().name().contains(m_searchQuery, Qt::CaseInsensitive) ||
                      asset.currency().code().contains(m_searchQuery, Qt::CaseInsensitive) ||
                      asset.currency().symbol().contains(m_searchQuery, Qt::CaseInsensitive);

        return result;
    }, [](const QModelIndex& sourceLeft, const QModelIndex& sourceRight) -> bool {
        const auto& leftEntry  = sourceLeft.data(Models::AssetsModel::AssetRole).value<Data::Asset>();
        const auto& rightEntry = sourceRight.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

        if (leftEntry.balance() == rightEntry.balance())
        {
            return static_cast<qint32>(leftEntry.type()) < static_cast<qint32>(rightEntry.type());
        }

        return leftEntry.balance() > rightEntry.balance();
    }));

    m_sortedAssetsModel->sort(0);

    //////////////////////////////////////////////

    const auto& state  = Store::instance().state();
    const auto& assets = state.wallet().assets();

    m_assetsModel->setList(assets.values());

    //////////////////////////////////////////////

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        const auto& assets = state.wallet().assets();
        const auto& orders = state.wallet().orders();
        const auto& swaps  = state.wallet().swaps();

        m_assetsModel->setList(assets.values());

        for (const auto& asset : assets)
        {
            if (asset.type() == m_bidAsset.type())
            {
                setBidAsset(asset);
            }
            else if (asset.type() == m_askAsset.type())
            {
                setAskAsset(asset);
            }
        }
    });

    //////////////////////////////////////////////

    connect(Store::instance().walletMiddleware().data(), &Middleware::WalletMiddleware::orderCreated, this, [this](const WalletServiceActions::PlaceOrder&, const Data::Order&) {
        setBidValue(QString(), ChangeReason::StateChanged);
        setAskValue(QString(), ChangeReason::StateChanged);

        setStatus(Status::IdleStatus);
    });

    //////////////////////////////////////////////

    connect(this, &SwapViewModel::bidValueChanged, this, [this](const QString& value, ChangeReason changeReason) {
        const auto& coinsValue = prepareUserInput(value);

        if (coinsValue.isEmpty())
        {
            setAskValue(QString(), ChangeReason::Calculations);
            setFiatValue("0.00"_L1);
            return;
        }

        Data::Float100 coinsAmount = Data::Float100(coinsValue.toStdString());
        Data::Float100 fiatAmount  = coinsAmount * m_bidAsset.quote().price();

        setFiatValue(Data::toPrettyString(fiatAmount, 2, 2, true));

        if (changeReason == ChangeReason::Calculations ||
            m_focusedField == FocusedField::AskField)
        {
            return;
        }

        Data::Float100 availableCoins = m_bidAsset.smallCoinToBigCoinValue(Data::Int256(m_bidAsset.balance().confirmed()) + m_bidAsset.balance().unconfirmed());

        Errors newErrors = errors().setFlag(Error::InsufficientAmountError, coinsAmount > availableCoins);

        setErrors(newErrors);

        Data::Float100 bidFiat  = coinsAmount * m_bidAsset.quote().price();
        Data::Float100 askCoins = bidFiat / m_askAsset.quote().price();

        setAskValue(Data::toPrettyString(askCoins), ChangeReason::Calculations);
    });

    connect(this, &SwapViewModel::askValueChanged, this, [this](const QString& value, ChangeReason changeReason) {
        const auto& coinsValue = prepareUserInput(value);

        if (coinsValue.isEmpty())
        {
            setBidValue(QString(), ChangeReason::Calculations);
            return;
        }

        if (changeReason == ChangeReason::Calculations ||
            m_focusedField == FocusedField::BidField)
        {
            return;
        }

        Data::Float100 coinsAmount    = Data::Float100(coinsValue.toStdString());
        Data::Float100 availableCoins = m_bidAsset.smallCoinToBigCoinValue(Data::Int256(m_bidAsset.balance().confirmed()) + m_bidAsset.balance().unconfirmed());

        Data::Float100 askFiat  = coinsAmount * m_askAsset.quote().price();
        Data::Float100 bidCoins = askFiat / m_bidAsset.quote().price();

        Errors newErrors = errors().setFlag(Error::InsufficientAmountError, bidCoins > availableCoins);

        setErrors(newErrors);

        setBidValue(Data::toPrettyString(bidCoins), ChangeReason::Calculations);
    });

    //////////////////////////////////////////////

    connect(this, &SwapViewModel::bidAssetChanged, this, [this, swapAssets]() {
        if (m_askAsset.type() == Data::Asset::Type::Unknown)
        {
            qsizetype i = 0;

            do
            {
                i = std::rand() % m_assetsModel->list().size();
            }
            while (!(m_assetsModel->list().at(i).type() != m_bidAsset.type() &&
                     swapAssets.contains(m_assetsModel->list().at(i).type())));

            setAskAsset(m_assetsModel->list().at(i));
        }

        setAskValue(m_askValue, ChangeReason::StateChanged);
    });

    connect(this, &SwapViewModel::askAssetChanged, this, [this]() {
        setBidValue(m_bidValue, ChangeReason::StateChanged);
    });
}

void SwapViewModel::interchangeAssets()
{
    Data::Asset bidAsset = m_bidAsset;
    Data::Asset askAsset = m_askAsset;

    QString bidValue = m_bidValue;
    QString askValue = m_askValue;

    setBidAsset(askAsset);
    setAskAsset(bidAsset);

    setBidValue(askValue);
    setAskValue(bidValue);
}

void SwapViewModel::makeSwap()
{
    setStatus(Status::CreatingOrderStatus);

    Data::Order order;
    order.setBidAssetType(m_bidAsset.type());
    order.setAskAssetType(m_askAsset.type());
    order.setBidAmount(Data::Float100(prepareUserInput(m_bidValue).toStdString()));
    order.setAskAmount(Data::Float100(prepareUserInput(m_askValue).toStdString()));

    Store::instance().dispatch(WalletServiceActions::PlaceOrder { order });
}

void SwapViewModel::cancelOrder(const QString& orderId)
{
    Store::instance().dispatch(WalletServiceActions::CancelOrder { orderId });
}

QString SwapViewModel::prepareUserInput(const QString& userInput)
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

void SwapViewModel::setStatus(Status status)
{
    if (m_status == status)
    {
        return;
    }
    m_status = status;
    emit statusChanged();
}

void SwapViewModel::setErrors(Errors errors)
{
    if (m_errors == errors)
    {
        return;
    }
    m_errors = errors;
    emit errorsChanged();
}

void SwapViewModel::setFocusedField(FocusedField focusedField)
{
    if (m_focusedField == focusedField)
    {
        return;
    }
    m_focusedField = focusedField;
    emit focusedFieldChanged();
}

void SwapViewModel::setBidAsset(const Data::Asset& bidAsset)
{
    if (m_bidAsset == bidAsset)
    {
        return;
    }
    m_bidAsset = bidAsset;
    emit bidAssetChanged();
}

void SwapViewModel::setAskAsset(const Data::Asset& askAsset)
{
    if (m_askAsset == askAsset)
    {
        return;
    }
    m_askAsset = askAsset;
    emit askAssetChanged();
}

void SwapViewModel::setBidValue(const QString& bidValue, ChangeReason changeReason)
{
    if (m_bidValue == bidValue && changeReason != ChangeReason::StateChanged)
    {
        return;
    }
    m_bidValue = bidValue;
    emit bidValueChanged(bidValue, changeReason);
}

void SwapViewModel::setAskValue(const QString& askValue, ChangeReason changeReason)
{
    if (m_askValue == askValue && changeReason != ChangeReason::StateChanged)
    {
        return;
    }
    m_askValue = askValue;
    emit askValueChanged(askValue, changeReason);
}

void SwapViewModel::setFiatValue(const QString& fiatValue)
{
    if (m_fiatValue == fiatValue)
    {
        return;
    }
    m_fiatValue = fiatValue;
    emit fiatValueChanged();
}

void SwapViewModel::setFilterAsset(FilterAsset filterAsset)
{
    m_filterAsset = filterAsset;
    emit filterAssetChanged();

    m_sortedAssetsModel->invalidateFilter();
}

void SwapViewModel::setSearchQuery(const QString& searchQuery)
{
    if (m_searchQuery == searchQuery)
    {
        return;
    }
    m_searchQuery = searchQuery;
    emit searchQueryChanged();

    m_sortedAssetsModel->invalidateFilter();
}

} // namespace XWallet::ViewModels
