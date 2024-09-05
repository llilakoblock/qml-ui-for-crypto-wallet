#include "WalletViewModel.h"

#include "singletones/Store.h"

namespace XWallet::ViewModels {

WalletViewModel::WalletViewModel(QObject* parent)
    : QObject(parent)
    , m_assetsModel(new Models::AssetsModel)
{
    m_sortedAssetsModel.reset(new SortFilterProxyModelByCondition(m_assetsModel.data(), [this](const QModelIndex& index) -> bool {
        if (m_searchQuery.isEmpty())
        {
            return true;
        }

        const auto& asset = index.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

        bool result = asset.currency().name().contains(m_searchQuery, Qt::CaseInsensitive) ||
                      asset.currency().code().contains(m_searchQuery, Qt::CaseInsensitive) ||
                      asset.currency().symbol().contains(m_searchQuery, Qt::CaseInsensitive);

        return result;
    }, [this](const QModelIndex& sourceLeft, const QModelIndex& sourceRight) -> bool {
        const auto& leftEntry  = sourceLeft.data(Models::AssetsModel::AssetRole).value<Data::Asset>();
        const auto& rightEntry = sourceRight.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

        const auto& leftCoinBalance = leftEntry.smallCoinToBigCoinValue(Data::Int256(leftEntry.balance().confirmed()) + leftEntry.balance().unconfirmed());
        const auto& leftFiatBalance = leftCoinBalance * leftEntry.quote().price();

        const auto& rightCoinBalance = rightEntry.smallCoinToBigCoinValue(Data::Int256(rightEntry.balance().confirmed()) + rightEntry.balance().unconfirmed());
        const auto& rightFiatBalance = rightCoinBalance * rightEntry.quote().price();

        if (leftFiatBalance == rightFiatBalance)
        {
            return static_cast<qint32>(leftEntry.type()) < static_cast<qint32>(rightEntry.type());
        }

        return leftFiatBalance > rightFiatBalance;
    }));

    connect(m_sortedAssetsModel.data(), &QSortFilterProxyModel::modelReset, this, [this]() {
        qint32 count = m_sortedAssetsModel->rowCount();

        if (count == 0)
        {
            selectIndex(-1);
            return;
        }

        for (qint32 i = 0; i < count; i++)
        {
            const auto& index = m_sortedAssetsModel->index(i, 0);
            const auto& asset = index.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

            if (asset.type() == m_currentAsset.type())
            {
                selectIndex(i);
                return;
            }
        }

        selectIndex(-1);
    });

    connect(m_sortedAssetsModel.data(), &QSortFilterProxyModel::layoutChanged, this, [this]() {
        qint32 count = m_sortedAssetsModel->rowCount();

        if (count == 0)
        {
            selectIndex(-1);
            return;
        }

        for (qint32 i = 0; i < count; i++)
        {
            const auto& index = m_sortedAssetsModel->index(i, 0);
            const auto& asset = index.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

            if (asset.type() == m_currentAsset.type())
            {
                selectIndex(i);
                return;
            }
        }

        selectIndex(0);
    });

    m_sortedAssetsModel->sort(0);

    //////////////////////////////////////////////

    const auto& state  = Store::instance().state();
    const auto& assets = state.wallet().assets();

    // m_assetsModel->setList(assets.values());
    m_assetsModel->setList(assets.values());

    selectIndex(0);

    //////////////////////////////////////////////

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        const auto& assets = state.wallet().assets();

        m_assetsModel->setList(assets.values());

        for (const auto& asset : assets)
        {
            if (asset.type() == m_currentAsset.type())
            {
                setCurrentAsset(asset);
                break;
            }
        }

        selectIndex(m_currentIndex);
    });

    connect(this, &WalletViewModel::searchQueryChanged, this, [this]() {
        m_sortedAssetsModel->invalidateFilter();
    });
}

void WalletViewModel::selectIndex(qint32 index)
{
    setCurrentIndex(index);

    if (index < 0 || index >= m_sortedAssetsModel->rowCount())
    {
        return;
    }

    const auto& sourceIndex = m_sortedAssetsModel->index(index, 0);

    const auto& asset = sourceIndex.data(Models::AssetsModel::AssetRole).value<Data::Asset>();

    setCurrentAsset(asset);
}

void WalletViewModel::setSearchQuery(const QString& searchQuery)
{
    if (m_searchQuery == searchQuery)
    {
        return;
    }
    m_searchQuery = searchQuery;
    emit searchQueryChanged();
}

void WalletViewModel::setCurrentIndex(qint32 currentIndex)
{
    if (m_currentIndex == currentIndex)
    {
        return;
    }
    m_currentIndex = currentIndex;
    emit currentIndexChanged();
}

void WalletViewModel::setCurrentAsset(const Data::Asset& asset)
{
    if (m_currentAsset == asset)
    {
        return;
    }
    m_currentAsset = asset;
    emit currentAssetChanged();
}

} // namespace XWallet::ViewModels
