#include "ApplicationViewModel.h"

#include "singletones/Store.h"

#include <QFuture>

namespace XWallet::ViewModels {

ApplicationViewModel::ApplicationViewModel(QObject* parent)
    : QObject(parent)
{
    Store::instance().dispatch(ApplicationActions::ApplicationStateChanged { Data::Application::State::Initializing });

    const auto& initializingTimer = QSharedPointer<QTimer>::create();
    const auto& loadingTimer      = QSharedPointer<QTimer>::create();

    auto initializingTimerFuture = QtFuture::connect(initializingTimer.data(), &QTimer::timeout);
    auto loadingTimerFuture      = QtFuture::connect(loadingTimer.data(), &QTimer::timeout);

    initializingTimerFuture.then([this, initializingTimer, loadingTimer]() {
        Store::instance().dispatch(ApplicationActions::ApplicationStateChanged { Data::Application::State::Loading });
        loadingTimer->start(100);
    });

    loadingTimerFuture.then([this, initializingTimer, loadingTimer]() {
        Store::instance().dispatch(ApplicationActions::ApplicationStateChanged { Data::Application::State::Start });
    });

    initializingTimer->start(100);

    const auto& state = Store::instance().state();

    setApplication(state.application());
    setWallet(state.wallet());

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        setApplication(state.application());
        setWallet(state.wallet());
    });
}

void ApplicationViewModel::setApplication(const Data::Application& applicationState)
{
    if (m_applicationState == applicationState)
    {
        return;
    }
    m_applicationState = applicationState;
    emit applicationChanged();
}

void ApplicationViewModel::setWallet(const Data::Wallet& wallet)
{
    if (m_wallet == wallet)
    {
        return;
    }
    m_wallet = wallet;
    emit walletChanged();
}

} // namespace XWallet::ViewModels
