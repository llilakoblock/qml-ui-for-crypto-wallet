#pragma once

#include <QtQmlIntegration>

namespace XWallet {

namespace Biometric {

class FacetecServer;

}

namespace ViewModels {

class BiometricViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_DISABLE_COPY_MOVE(BiometricViewModel)

    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)

    Q_PROPERTY(QUrl serverUrl READ serverUrl CONSTANT FINAL)

public:
    enum class State
    {
        UnknownState,
        NoneExistingUserState,
        EmptyPassword,
        CapturingState,
        ErrorState,
        SuccessState,
    };
    Q_ENUM(State)

public:
    explicit BiometricViewModel(QObject* parent = nullptr);
    ~BiometricViewModel() override;

    Q_INVOKABLE void showMainView();
    Q_INVOKABLE void showEnrollView();
    Q_INVOKABLE void showAuthenticateView(const QString& password);

private:
    State state() const { return m_state; }
    void  setState(State state);

    QUrl serverUrl() const;

    bool isUserPwInDb(const QString& password);

signals:
    void stateChanged();

private:
    QScopedPointer<Biometric::FacetecServer> m_biometricServer;

    State m_state { State::UnknownState };
};

} // namespace ViewModels
} // namespace XWallet
