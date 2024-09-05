#pragma once

#include "data/Step.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class StartupViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Data::Step step READ step NOTIFY stepChanged FINAL)

public:
    enum class PasswordError
    {
        None                        = 0,
        MinimumLengthError          = 1 << 0,
        WithoutCapitalLetterError   = 1 << 1,
        WithoutLowercaseLetterError = 1 << 2,
        WithoutNumbersError         = 1 << 3,
    };
    Q_DECLARE_FLAGS(PasswordErrors, PasswordError)
    Q_FLAG(PasswordError)

public:
    explicit StartupViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen) const;
    Q_INVOKABLE void finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QString& password) const;
    Q_INVOKABLE void finishScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QList<QString>& passphrase) const;

    Q_INVOKABLE void finishInheritanceScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, const QString& key) const;
    Q_INVOKABLE void finishInheritanceScreen(Data::Step::Screen currentScreen, Data::Step::Screen nextScreen, int InactivityDuration) const;

    static Q_INVOKABLE void nextScreen();
    static Q_INVOKABLE void previousScreen();

    static Q_INVOKABLE PasswordErrors validatePassword(const QString& password);

private:
    Data::Step step() const { return m_step; }
    void       setStep(const Data::Step& step);

signals:
    void stepChanged();

private:
    Data::Step m_step;
};

} // namespace XWallet::ViewModels
