#pragma once

#include <QSortFilterProxyModel>

#include <functional>

namespace XWallet {

class SortFilterProxyModelByCondition : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int length READ length NOTIFY lengthChanged)

public:
    using FilterConditionT  = std::function<bool(const QModelIndex& index)>;
    using SortingConditionT = std::function<bool(const QModelIndex& sourceLeft, const QModelIndex& sourceRight)>;

    SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, FilterConditionT&& condition, QObject* parent = nullptr);
    SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, SortingConditionT&& condition, QObject* parent = nullptr);
    SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, SortingConditionT& condition, QObject* parent = nullptr);
    SortFilterProxyModelByCondition(QAbstractItemModel* sourceModel, FilterConditionT&& filterCondition, SortingConditionT&& sortingCondition, QObject* parent = nullptr);

public slots:
    void invalidateFilter();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
    bool lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const override;

private:
    void makeConnections();

    int length() const { return rowCount(); }

signals:
    void lengthChanged();

private:
    FilterConditionT  m_filterCondition;
    SortingConditionT m_sortingCondition;
};

} // namespace XWallet
