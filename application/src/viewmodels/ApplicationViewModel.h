#pragma once

#include "data/Application.h"
#include "data/Wallet.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class ApplicationViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_DISABLE_COPY_MOVE(ApplicationViewModel)

    Q_PROPERTY(Data::Application application READ application NOTIFY applicationChanged FINAL)
    Q_PROPERTY(Data::Wallet wallet READ wallet NOTIFY walletChanged FINAL)

public:
    explicit ApplicationViewModel(QObject* parent = nullptr);

private:
    Data::Application application() const { return m_applicationState; }
    void              setApplication(const Data::Application& applicationState);

    Data::Wallet wallet() const { return m_wallet; }
    void         setWallet(const Data::Wallet& wallet);

signals:
    void applicationChanged();

    void walletChanged();

private:
    Data::Application m_applicationState;

    Data::Wallet m_wallet;
};

} // namespace XWallet::ViewModels
