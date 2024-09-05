#include "OrdersModel.h"

#include "singletones/Store.h"
#include "data/NumericTypes.h"
#include "data/Swap.h"

namespace XWallet::Models {

OrdersModel::OrdersModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(this, &OrdersModel::modelReset, this, &OrdersModel::rowCountChanged);
    connect(this, &OrdersModel::rowsInserted, this, &OrdersModel::rowCountChanged);
    connect(this, &OrdersModel::rowsRemoved, this, &OrdersModel::rowCountChanged);
}

void OrdersModel::setList(const QList<Data::Order>& list)
{
    QList<Data::Swap> swaps;

    for (const auto& order : list)
    {
        if (order.creator() == Data::Order::Creator::Me)
        {
            Data::Swap swap;
            swap.setId(order.id());
            swap.setTimestamp(QDateTime::currentDateTime());
            swap.setMyOrder(order);

            Data::Order otherOrder = order;
            otherOrder.setCreator(Data::Order::Creator::Other);
            otherOrder.setBidAssetType(order.askAssetType());
            otherOrder.setBidAmount(order.askAmount());
            otherOrder.setAskAssetType(order.bidAssetType());
            otherOrder.setAskAmount(order.bidAmount());

            swap.setOtherOrder(otherOrder);

            swaps.append(swap);
        }
    }

    if (m_list == swaps)
    {
        return;
    }

    for (int i = 0; i < swaps.size(); i++)
    {
        const auto& newItem = swaps.at(i);

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

    if (swaps.size() < m_list.size())
    {
        beginRemoveRows(QModelIndex(), swaps.size(), m_list.size() - 1);

        m_list = swaps;

        endRemoveRows();
    }
}

void OrdersModel::setAssets(const Data::Asset::Hash& assets)
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

int OrdersModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_list.count();
}

QVariant OrdersModel::data(const QModelIndex& index, int role) const
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
        default: return QVariant();
    }
}

bool OrdersModel::setData(const QModelIndex& index, const QVariant& value, int role)
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

QHash<int, QByteArray> OrdersModel::roleNames() const
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
