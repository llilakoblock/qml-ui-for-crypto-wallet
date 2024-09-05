#include "HistoryViewModel.h"

#include "singletones/Store.h"

namespace XWallet::ViewModels {

HistoryViewModel::HistoryViewModel(QObject* parent)
    : QObject(parent)
{
    m_sortedHistoryModel.reset(new SortFilterProxyModelByCondition(nullptr, [this](const QModelIndex& index) -> bool {
        const auto& historyEntry = index.data(Models::HistoryModel::HistoryEntryRole).value<Data::HistoryEntry>();

        switch (m_filteringKind)
        {
            case FilteringKind::AllKind:
            {
                break;
            }
            case FilteringKind::ReceivedKind:
            {
                if (historyEntry.direction() != Data::HistoryEntry::Direction::Income)
                {
                    return false;
                }
                break;
            }
            case FilteringKind::SentKind:
            {
                if (historyEntry.direction() != Data::HistoryEntry::Direction::Outgo)
                {
                    return false;
                }
                break;
            }
        }

        const auto& timestamp   = historyEntry.timestamp();
        const auto& currentDate = QDateTime::currentDateTime();

        switch (m_filteringDate)
        {
            case FilteringDate::AllDate:
            {
                break;
            }
            case FilteringDate::TodayDate:
            {
                if (timestamp.date() != currentDate.date())
                {
                    return false;
                }
                break;
            }
        }

        return true;
    }, [this](const QModelIndex& sourceLeft, const QModelIndex& sourceRight) -> bool {
        const auto& leftEntry  = sourceLeft.data(Models::HistoryModel::HistoryEntryRole).value<Data::HistoryEntry>();
        const auto& rightEntry = sourceRight.data(Models::HistoryModel::HistoryEntryRole).value<Data::HistoryEntry>();

        return leftEntry.timestamp() > rightEntry.timestamp();
    }));

    m_sortedHistoryModel->sort(0);

    //////////////////////////////////////////////

    const auto& state  = Store::instance().state();
    const auto& assets = state.wallet().assets();

    for (const auto& asset : assets)
    {
        m_historyModels.insert(asset.type(), QSharedPointer<Models::HistoryModel>::create());
        m_historyModels.value(asset.type())->setList(asset.history());
    }

    handleSelectedAssetType();

    //////////////////////////////////////////////

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        const auto& assets = state.wallet().assets();

        for (const auto& asset : assets)
        {
            if (!m_historyModels.contains(asset.type()))
            {
                m_historyModels.insert(asset.type(), QSharedPointer<Models::HistoryModel>::create());
            }

            m_historyModels.value(asset.type())->setList(asset.history());
        }

        handleSelectedAssetType();
    });

    connect(this, &HistoryViewModel::selectedAssetTypeChanged, this, &HistoryViewModel::handleSelectedAssetType);
}

void HistoryViewModel::leaveNote(const QString& transactionId, const QString& note)
{
    const auto& asset = Store::instance().state().wallet().assets().value(m_selectedAssetType);

    QList<Data::HistoryEntry> history = asset.history();

    if (history.isEmpty())
    {
        return;
    }

    for (qsizetype i = 0; i < history.size(); i++)
    {
        if (history.at(i).hash() == transactionId)
        {
            auto newHistoryEntry = history.at(i);
            newHistoryEntry.setNote(note);

            history.replace(i, newHistoryEntry);
            break;
        }
    }

    Store::instance().dispatch(DatabaseServiceActions::StoreHistoryNotes { m_selectedAssetType,
                                                                           history });
}

void HistoryViewModel::setSelectedAssetType(Data::Asset::Type selectedAssetType)
{
    if (m_selectedAssetType == selectedAssetType)
    {
        return;
    }
    m_selectedAssetType = selectedAssetType;
    emit selectedAssetTypeChanged();
}

void HistoryViewModel::setFilteringKind(FilteringKind filteringKind)
{
    if (m_filteringKind == filteringKind)
    {
        return;
    }
    m_filteringKind = filteringKind;
    emit filteringKindChanged();

    m_sortedHistoryModel->invalidateFilter();
}

void HistoryViewModel::setFilteringDate(FilteringDate filteringDate)
{
    if (m_filteringDate == filteringDate)
    {
        return;
    }
    m_filteringDate = filteringDate;
    emit filteringDateChanged();

    m_sortedHistoryModel->invalidateFilter();
}

void HistoryViewModel::handleSelectedAssetType()
{
    if (m_selectedAssetType == Data::Asset::Type::Unknown)
    {
        return;
    }

    const auto& model = m_historyModels.value(m_selectedAssetType);

    if (!model)
    {
        return;
    }

    m_sortedHistoryModel->setSourceModel(model.data());
}

} // namespace XWallet::ViewModels
