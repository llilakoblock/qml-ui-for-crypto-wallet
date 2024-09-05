#pragma once

#include "misc/SortFilterProxyModelByCondition.h"
#include "models/ConcatenatedSwapsModel.h"
#include "models/OrdersModel.h"
#include "models/SwapsModel.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class SwapHistoryViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(FilteringKind filteringKind READ filteringKind WRITE setFilteringKind NOTIFY filteringKindChanged FINAL)
    Q_PROPERTY(FilteringDate filteringDate READ filteringDate WRITE setFilteringDate NOTIFY filteringDateChanged FINAL)

    Q_PROPERTY(QAbstractItemModel* swapsModel READ swapsModel CONSTANT FINAL)

public:
    enum class FilteringKind
    {
        AllKind        = 0,
        ProcessingKind = 1,
        CompletedKind  = 2,
    };
    Q_ENUM(FilteringKind)

    enum class FilteringDate
    {
        AllDate   = 0,
        TodayDate = 1,
    };
    Q_ENUM(FilteringDate)

public:
    explicit SwapHistoryViewModel(QObject* parent = nullptr);

private:
    FilteringKind filteringKind() const { return m_filteringKind; }
    void          setFilteringKind(FilteringKind filteringKind);

    FilteringDate filteringDate() const { return m_filteringDate; }
    void          setFilteringDate(FilteringDate filteringDate);

    QAbstractItemModel* swapsModel() const { return m_sortedSwapsModel.data(); }

signals:
    void filteringKindChanged();
    void filteringDateChanged();

private:
    FilteringKind m_filteringKind { FilteringKind::AllKind };
    FilteringDate m_filteringDate { FilteringDate::AllDate };

    QScopedPointer<Models::OrdersModel> m_ordersModel;
    QScopedPointer<Models::SwapsModel>  m_swapsModel;

    QScopedPointer<SortFilterProxyModelByCondition> m_sortedSwapsModel;

    QScopedPointer<Models::ConcatenatedSwapsModel> m_concatenatedSwapsModel;
};

} // namespace XWallet::ViewModels
