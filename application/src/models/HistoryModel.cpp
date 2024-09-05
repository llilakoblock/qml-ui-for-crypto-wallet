#include "HistoryModel.h"

namespace XWallet::Models {

HistoryModel::HistoryModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(this, &HistoryModel::modelReset, this, &HistoryModel::lengthChanged);
    connect(this, &HistoryModel::rowsInserted, this, &HistoryModel::lengthChanged);
    connect(this, &HistoryModel::rowsRemoved, this, &HistoryModel::lengthChanged);
}

void HistoryModel::setList(const QList<Data::HistoryEntry>& list)
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

int HistoryModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_list.count();
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case HistoryEntryRole:
            return QVariant::fromValue(item);
        case DateRole:
            return item.timestamp().date() == QDate::currentDate() ?
                       tr("Today") :
                       item.timestamp().toString(QStringLiteral("MMM dd, yyyy"));
        case ExpandedRole:
            return m_expandedItems.value(item.hash(), false);
        default:
            return QVariant();
    }
}

bool HistoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return false;
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case ExpandedRole:
            m_expandedItems.insert(item.hash(), value.toBool());
            break;
        default:
            return false;
    }

    emit dataChanged(index, index, { role });

    return true;
}

QHash<int, QByteArray> HistoryModel::roleNames() const
{
    return {
        { HistoryEntryRole, "historyEntry" },
        { DateRole,         "date"         },
        { ExpandedRole,     "expanded"     }
    };
}

} // namespace XWallet::Models
