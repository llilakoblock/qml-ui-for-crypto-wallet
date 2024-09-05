#include "SwapHistoryViewModel.h"

#include "singletones/Store.h"

namespace XWallet::ViewModels {

SwapHistoryViewModel::SwapHistoryViewModel(QObject* parent)
    : QObject(parent)
    , m_ordersModel(new Models::OrdersModel)
    , m_swapsModel(new Models::SwapsModel)
    , m_concatenatedSwapsModel(new Models::ConcatenatedSwapsModel)
{
    m_concatenatedSwapsModel->addSourceModel(m_ordersModel.data());
    m_concatenatedSwapsModel->addSourceModel(m_swapsModel.data());

    m_sortedSwapsModel.reset(new SortFilterProxyModelByCondition(m_concatenatedSwapsModel.data(), [this](const QModelIndex& index) -> bool {
        const auto& swap = index.data(Models::SwapsModel::SwapRole).value<Data::Swap>();

        switch (m_filteringKind)
        {
            case FilteringKind::AllKind:
            {
                break;
            }
            case FilteringKind::ProcessingKind:
            {
                if (swap.state() >= Data::Swap::State::FinishedState)
                {
                    return false;
                }
                break;
            }
            case FilteringKind::CompletedKind:
            {
                if (swap.state() < Data::Swap::State::FinishedState)
                {
                    return false;
                }
                break;
            }
        }

        const auto& timestamp   = swap.timestamp();
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
    }, [](const QModelIndex& sourceLeft, const QModelIndex& sourceRight) -> bool {
        const auto& leftEntry  = sourceLeft.data(Models::SwapsModel::SwapRole).value<Data::Swap>();
        const auto& rightEntry = sourceRight.data(Models::SwapsModel::SwapRole).value<Data::Swap>();

        return leftEntry.timestamp() > rightEntry.timestamp();
    }));

    m_sortedSwapsModel->sort(0);

    //////////////////////////////////////////////

    const auto& state  = Store::instance().state();
    const auto& assets = state.wallet().assets();
    const auto& orders = state.wallet().orders();
    const auto& swaps  = state.wallet().swaps();

    m_ordersModel->setList(orders.values());
    m_ordersModel->setAssets(assets);

    m_swapsModel->setList(swaps.values());
    m_swapsModel->setAssets(assets);

    //////////////////////////////////////////////

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        const auto& assets = state.wallet().assets();
        const auto& orders = state.wallet().orders();
        const auto& swaps  = state.wallet().swaps();

        m_ordersModel->setList(orders.values());
        m_ordersModel->setAssets(assets);

        m_swapsModel->setList(swaps.values());
        m_swapsModel->setAssets(assets);
    });
}

void SwapHistoryViewModel::setFilteringKind(FilteringKind filteringKind)
{
    if (m_filteringKind == filteringKind)
    {
        return;
    }
    m_filteringKind = filteringKind;
    emit filteringKindChanged();

    m_sortedSwapsModel->invalidateFilter();
}

void SwapHistoryViewModel::setFilteringDate(FilteringDate filteringDate)
{
    if (m_filteringDate == filteringDate)
    {
        return;
    }
    m_filteringDate = filteringDate;
    emit filteringDateChanged();

    m_sortedSwapsModel->invalidateFilter();
}

} // namespace XWallet::ViewModels
