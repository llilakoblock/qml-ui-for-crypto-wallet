#pragma once

#include "data/Asset.h"
#include "misc/SortFilterProxyModelByCondition.h"
#include "models/HistoryModel.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class HistoryViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Data::Asset::Type selectedAssetType READ selectedAssetType WRITE setSelectedAssetType NOTIFY selectedAssetTypeChanged REQUIRED FINAL)

    Q_PROPERTY(FilteringKind filteringKind READ filteringKind WRITE setFilteringKind NOTIFY filteringKindChanged FINAL)
    Q_PROPERTY(FilteringDate filteringDate READ filteringDate WRITE setFilteringDate NOTIFY filteringDateChanged FINAL)

    Q_PROPERTY(QAbstractItemModel* historyModel READ historyModel CONSTANT FINAL)

public:
    enum class FilteringKind
    {
        AllKind      = 0,
        SentKind     = 1,
        ReceivedKind = 2,
    };
    Q_ENUM(FilteringKind)

    enum class FilteringDate
    {
        AllDate   = 0,
        TodayDate = 1,
    };
    Q_ENUM(FilteringDate)

public:
    explicit HistoryViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void leaveNote(const QString& transactionId, const QString& note);

private:
    Data::Asset::Type selectedAssetType() const { return m_selectedAssetType; }
    void              setSelectedAssetType(Data::Asset::Type selectedAssetType);

    FilteringKind filteringKind() const { return m_filteringKind; }
    void          setFilteringKind(FilteringKind filteringKind);

    FilteringDate filteringDate() const { return m_filteringDate; }
    void          setFilteringDate(FilteringDate filteringDate);

    QAbstractItemModel* historyModel() const { return m_sortedHistoryModel.data(); }

    void handleSelectedAssetType();

signals:
    void selectedAssetTypeChanged();

    void filteringChanged();

    void filteringKindChanged();

    void filteringDateChanged();

private:
    Data::Asset::Type m_selectedAssetType { Data::Asset::Type::Unknown };

    FilteringKind m_filteringKind { FilteringKind::AllKind };
    FilteringDate m_filteringDate { FilteringDate::AllDate };

    QScopedPointer<SortFilterProxyModelByCondition> m_sortedAssetsModel;
    QScopedPointer<SortFilterProxyModelByCondition> m_sortedHistoryModel;

    QHash<Data::Asset::Type, QSharedPointer<Models::HistoryModel>> m_historyModels;
};

} // namespace XWallet::ViewModels
