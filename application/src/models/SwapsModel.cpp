#include "SwapsModel.h"

namespace XWallet::Models {

SwapsModel::SwapsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(this, &SwapsModel::modelReset, this, &SwapsModel::rowCountChanged);
    connect(this, &SwapsModel::rowsInserted, this, &SwapsModel::rowCountChanged);
    connect(this, &SwapsModel::rowsRemoved, this, &SwapsModel::rowCountChanged);
}

void SwapsModel::setList(const QList<Data::Swap>& list)
{
    if (m_list == list)
    {
        return;
    }

    for (int i = 0; i < list.size(); i++)
    {
        const auto& newItem = list.at(i);

        if (i >= m_list.size())
        {
            beginInsertRows(QModelIndex(), i, i);

            m_list.append(newItem);

            endInsertRows();

            continue;
        }

        const auto& oldItem = m_list.at(i);

        if (newItem == oldItem)
        {
            continue;
        }

        m_list.replace(i, newItem);

        QModelIndex index = createIndex(i, 0);

        emit dataChanged(index, index);
    }

    if (list.size() < m_list.size())
    {
        beginRemoveRows(QModelIndex(), list.size(), m_list.size() - 1);

        m_list = list;

        endRemoveRows();
    }
}

void SwapsModel::setAssets(const Data::Asset::Hash& assets)
{
    if (m_assets == assets)
    {
        return;
    }

    m_assets = assets;

    for (int i = 0; i < m_list.size(); i++)
    {
        const auto& item = m_list.at(i);

        if (assets.contains(item.myOrder().askAssetType()) ||
            assets.contains(item.myOrder().bidAssetType()) ||
            assets.contains(item.otherOrder().askAssetType()) ||
            assets.contains(item.otherOrder().bidAssetType()))
        {
            QModelIndex index = createIndex(i, 0);

            emit dataChanged(index, index);
        }
    }
}

int SwapsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_list.count();
}

QVariant SwapsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case SwapRole:
            return QVariant::fromValue(item);
        case BidAssetRole:
            return QVariant::fromValue(m_assets.value(item.myOrder().bidAssetType()));
        case AskAssetRole:
            return QVariant::fromValue(m_assets.value(item.myOrder().askAssetType()));
        case BidCoinAmountRole:
            return Data::toPrettyString(item.myOrder().bidAmount());
        case BidFiatAmountRole:
        {
            const auto& asset = m_assets.value(item.myOrder().bidAssetType());

            return Data::toPrettyString(item.myOrder().bidAmount() * asset.quote().price(), 2, 2);
        }
        case AskCoinAmountRole:
            return Data::toPrettyString(item.myOrder().askAmount());
        case AskFiatAmountRole:
        {
            const auto& asset = m_assets.value(item.myOrder().askAssetType());

            return Data::toPrettyString(item.myOrder().askAmount() * asset.quote().price(), 2, 2);
        }
        case ExpandedRole:
            return m_expandedItems.value(item.id(), false);
        case ShowLogsRole:
            return m_showLogsItems.value(item.id(), false);
        default:
            return QVariant();
    }
}

bool SwapsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return false;
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case ExpandedRole:
            m_expandedItems.insert(item.id(), value.toBool());
            break;
        case ShowLogsRole:
            m_showLogsItems.insert(item.id(), value.toBool());
            break;
        default:
            return false;
    }

    emit dataChanged(index, index, { role });

    return true;
}

QHash<int, QByteArray> SwapsModel::roleNames() const
{
    return {
        { SwapRole,          "swap"          },
        { BidAssetRole,      "bidAsset"      },
        { AskAssetRole,      "askAsset"      },
        { BidCoinAmountRole, "bidCoinAmount" },
        { BidFiatAmountRole, "bidFiatAmount" },
        { AskCoinAmountRole, "askCoinAmount" },
        { AskFiatAmountRole, "askFiatAmount" },
        { ExpandedRole,      "expanded"      },
        { ShowLogsRole,      "showLogs"      },
    };
}

} // namespace XWallet::Models
