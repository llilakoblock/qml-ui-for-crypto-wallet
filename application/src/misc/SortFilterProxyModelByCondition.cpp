#include "SortFilterProxyModelByCondition.h"

namespace XWallet {

SortFilterProxyModelByCondition::SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, FilterConditionT&& condition, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_filterCondition(std::move(condition))
{
    if (sourceModel)
    {
        setSourceModel(sourceModel);
    }

    makeConnections();
}

SortFilterProxyModelByCondition::SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, SortingConditionT&& condition, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_sortingCondition(std::move(condition))
{
    if (sourceModel)
    {
        setSourceModel(sourceModel);
    }

    makeConnections();
}

SortFilterProxyModelByCondition::SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, SortingConditionT& condition, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_sortingCondition(condition)
{
    if (sourceModel)
    {
        setSourceModel(sourceModel);
    }

    makeConnections();
}

SortFilterProxyModelByCondition::SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, FilterConditionT&& filterCondition, SortingConditionT&& sortingCondition, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_filterCondition(std::move(filterCondition))
    , m_sortingCondition(std::move(sortingCondition))
{
    if (sourceModel)
    {
        setSourceModel(sourceModel);
    }

    makeConnections();
}

void SortFilterProxyModelByCondition::invalidateFilter()
{
    emit layoutAboutToBeChanged();

    QSortFilterProxyModel::invalidateFilter();

    emit layoutChanged();
}

bool SortFilterProxyModelByCondition::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (m_filterCondition)
    {
        return m_filterCondition(sourceModel()->index(sourceRow, 0));
    }
    else
    {
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
}

bool SortFilterProxyModelByCondition::lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const
{
    if (m_sortingCondition)
    {
        return m_sortingCondition(sourceLeft, sourceRight);
    }
    else
    {
        return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
    }
}

void SortFilterProxyModelByCondition::makeConnections()
{
    connect(this, &QSortFilterProxyModel::rowsInserted, this, &SortFilterProxyModelByCondition::lengthChanged);
    connect(this, &QSortFilterProxyModel::rowsRemoved, this, &SortFilterProxyModelByCondition::lengthChanged);
    connect(this, &QSortFilterProxyModel::modelReset, this, &SortFilterProxyModelByCondition::lengthChanged);
    connect(this, &QSortFilterProxyModel::layoutChanged, this, &SortFilterProxyModelByCondition::lengthChanged);
}

} // namespace XWallet
