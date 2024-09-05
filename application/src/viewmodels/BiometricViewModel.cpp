#include "BiometricViewModel.h"

#include "biometric/FacetecServer.h"
#include "singletones/Store.h"
#include "freedomdatabase.h"

namespace XWallet::ViewModels {

BiometricViewModel::BiometricViewModel(QObject* parent)
    : QObject(parent)
    , m_biometricServer(new Biometric::FacetecServer)
{
    const auto& hash = QString(QCryptographicHash::hash(Store::instance().state().step().password().toStdString(), QCryptographicHash::Md5).toHex());

    m_biometricServer->setUserHash(hash);

    connect(&Store::instance(), &Store::stateChanged, this, [this](const Data::State& state) {
        const auto& hash = QString(QCryptographicHash::hash(state.step().password().toStdString(), QCryptographicHash::Md5).toHex());

        m_biometricServer->setUserHash(hash);
    });

    connect(m_biometricServer.data(), &Biometric::FacetecServer::statusReceived, this, [this](Biometric::FacetecServer::Status status) {
        switch (status)
        {
            case Biometric::FacetecServer::Status::EnrollSuccess:
            case Biometric::FacetecServer::Status::AuthenticateSuccess:
                setState(State::SuccessState);
                break;
            case Biometric::FacetecServer::Status::EnrollFail:
            case Biometric::FacetecServer::Status::EnrollCancel:
            case Biometric::FacetecServer::Status::AuthenticateFail:
            case Biometric::FacetecServer::Status::AuthenticateCancel:
                setState(State::ErrorState);
                break;
            default:
                setState(State::UnknownState);
                break;
        }
    });
}

BiometricViewModel::~BiometricViewModel() = default;

void BiometricViewModel::showMainView()
{
    setState(State::UnknownState);

    m_biometricServer->showMainView();
}

void BiometricViewModel::showEnrollView()
{
    setState(State::CapturingState);

    m_biometricServer->showEnrollView();
}

void BiometricViewModel::showAuthenticateView(const QString& password)
{
    if (password.isEmpty())
    {
        setState(State::EmptyPassword);
        return;
    }

    if (!isUserPwInDb(password))
    {
        setState(State::NoneExistingUserState);
        return;
    }

    setState(State::CapturingState);

    const auto& hash = QString(QCryptographicHash::hash(password.toStdString(), QCryptographicHash::Md5).toHex());

    m_biometricServer->setUserHash(hash);

    m_biometricServer->showAuthenticateView();
}

bool BiometricViewModel::isUserPwInDb(const QString& password)
{
    // Make sure hashedUserPw is found in db, otherwise no sense in doing the scan
    freedom::core::database::FreedomDatabase               database;
    std::optional<std::vector<std::vector<unsigned char>>> encryptedPassphrases;

    const std::string hashedUserPw = QCryptographicHash::hash(password.toStdString(), QCryptographicHash::Md5).toHex().toStdString();

    bool isUserPwDuplicate = database.isUserPwDuplicate(hashedUserPw);

    return isUserPwDuplicate;
}

void BiometricViewModel::setState(State state)
{
    if (m_state == state)
    {
        return;
    }
    m_state = state;
    emit stateChanged();
}

QUrl BiometricViewModel::serverUrl() const
{
    return m_biometricServer->url();
}

} // namespace XWallet::ViewModels
