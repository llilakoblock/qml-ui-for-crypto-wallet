#include "AssetsModel.h"

namespace XWallet::Models {

AssetsModel::AssetsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(this, &AssetsModel::modelReset, this, &AssetsModel::rowCountChanged);
    connect(this, &AssetsModel::rowsInserted, this, &AssetsModel::rowCountChanged);
    connect(this, &AssetsModel::rowsRemoved, this, &AssetsModel::rowCountChanged);
}

void AssetsModel::setList(const QList<Data::Asset>& list)
{
    if (m_list.isEmpty())
    {
        beginResetModel();

        m_list = list;

        endResetModel();

        return;
    }

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

int AssetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_list.count();
}

QVariant AssetsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case AssetRole:
            return QVariant::fromValue(item);
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> AssetsModel::roleNames() const
{
    return {
        { AssetRole, "asset" },
    };
}

} // namespace XWallet::Models
