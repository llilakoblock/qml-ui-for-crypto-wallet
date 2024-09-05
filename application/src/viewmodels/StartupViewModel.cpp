#include "StartupViewModel.h"

#include "actions/StepActions.h"
#include "singletones/ApplicationSettings.h"
#include "singletones/Store.h"

namespace XWallet::ViewModels {

constexpr qint64 MinimumPasswordLength = 8;

StartupViewModel::StartupViewModel(QObject* parent)
    : QObject(parent)
{
    const auto& state = Store::instance().state();

    setStep(state.step());

    if (state.step().screen() == Data::Step::Screen::Unlock && !Singletones::ApplicationSettings::instance().password().isEmpty())
    {
        Store::instance().dispatch(StepActions::FinishScreen {
            Data::Step::Screen::Unlock,
            Data::Step::Screen::Unknown,
            StepActions::PasswordData { Singletones::ApplicationSettings::instance().password() } });
    }

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        setStep(state.step());
    });
}

void StartupViewModel::finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen) const
{
    Store::instance().dispatch(StepActions::FinishScreen {
        currentScreen,
        nextScreen,
        StepActions::EmptyData {} });
}

void StartupViewModel::finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QString& password) const
{
    Store::instance().dispatch(StepActions::FinishScreen {
        currentScreen,
        nextScreen,
        StepActions::PasswordData { password } });
}

void StartupViewModel::finishInheritanceScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QString& key) const
{
    Store::instance().dispatch(StepActions::FinishScreen {
        currentScreen,
        nextScreen,
        StepActions::InheritanceKeyData { key } });
}

void StartupViewModel::finishInheritanceScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, int InactivityDuration) const
{
    Store::instance().dispatch(StepActions::FinishScreen {
        currentScreen,
        nextScreen,
        StepActions::InheritanceInactivityDurationData { InactivityDuration } });
}

void StartupViewModel::finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QList<QString>& passphrase) const
{
    Store::instance().dispatch(StepActions::FinishScreen {
        currentScreen,
        nextScreen,
        StepActions::PassphraseData { passphrase } });
}

void StartupViewModel::nextScreen()
{
    Store::instance().dispatch(StepActions::NextScreen {});
}

void StartupViewModel::previousScreen()
{
    Store::instance().dispatch(StepActions::PreviousScreen {});
}

StartupViewModel::PasswordErrors StartupViewModel::validatePassword(const QString& password)
{
    PasswordErrors errors = PasswordError::None;

    if (password.isEmpty() || password.length() < MinimumPasswordLength)
    {
        errors |= PasswordError::MinimumLengthError;
    }

    if (!password.contains(QRegularExpression("[A-ZА-Я]")))
    {
        errors |= PasswordError::WithoutCapitalLetterError;
    }

    if (!password.contains(QRegularExpression("[a-zа-я]")))
    {
        errors |= PasswordError::WithoutLowercaseLetterError;
    }

    if (!password.contains(QRegularExpression("[0-9]")))
    {
        errors |= PasswordError::WithoutNumbersError;
    }

    return errors;
}

void StartupViewModel::setStep(const Data::Step& step)
{
    if (m_step == step)
    {
        return;
    }

    m_step = step;
    emit stepChanged();
}

} // namespace XWallet::ViewModels
