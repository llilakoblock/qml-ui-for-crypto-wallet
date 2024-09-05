#pragma once

#include "actions/RootActions.h"
#include "data/State.h"

#include <QMutex>
#include <QThread>

namespace XWallet {

namespace Middleware {
class LoggingMiddleware;
class DatabaseMiddleware;
class WalletMiddleware;
class NetworkMiddleware;
class TaskMiddleware;
} // namespace Middleware
class Worker;

class Store final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Store)

public:
    static Store& instance();

    QFuture<void> dispatch(const RootActions::Action& action);

    Data::State state() const;

    QSharedPointer<Middleware::LoggingMiddleware>  loggingMiddleware() const;
    QSharedPointer<Middleware::DatabaseMiddleware> databaseMiddleware() const;
    QSharedPointer<Middleware::WalletMiddleware>   walletMiddleware() const;
    QSharedPointer<Middleware::NetworkMiddleware>  networkMiddleware() const;
    QSharedPointer<Middleware::TaskMiddleware>     taskMiddleware() const;

private:
    Store(QObject* parent = nullptr);
    ~Store() override;

signals:
    void launched();

    void actionAdded(const RootActions::Action& action, QSharedPointer<QPromise<void>> promise);
    void stateChanged(const Data::State& state);

private:
    QScopedPointer<QThread> m_thread;
    QScopedPointer<Worker>  m_worker;
};

} // namespace XWallet
