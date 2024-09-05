#include "singletones/Store.h"

#include "middleware/DatabaseMiddleware.h"
#include "middleware/LoggingMiddleware.h"
#include "middleware/NetworkMiddleware.h"
#include "middleware/TaskMiddleware.h"
#include "middleware/WalletMiddleware.h"
#include "reducers/StateReducer.h"

namespace XWallet {

class Worker : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Worker)

public:
    Worker(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    ~Worker() override = default;

    void applyAction(const RootActions::Action& action, const QSharedPointer<QPromise<void>>& promise)
    {
        promise->start();

        auto newAction = action;

        newAction = m_loggingMiddleware->apply(newAction);
        newAction = m_databaseMiddleware->apply(newAction);
        newAction = m_walletMiddleware->apply(newAction);
        newAction = m_networkMiddleware->apply(newAction);
        newAction = m_taskMiddleware->apply(newAction);

        if (std::holds_alternative<RootActions::EmptyAction>(newAction))
        {
            promise->finish();
            return;
        }

        if (std::holds_alternative<StateActions::Action>(newAction))
        {
            const auto& stateAction = std::get<StateActions::Action>(newAction);

            const auto& newState = Reducers::StateReducer::reduce(m_state, stateAction);

            if (newState == m_state)
            {
                promise->finish();
                return;
            }

            m_state = newState;

            emit stateChanged(m_state);
        }

        promise->finish();
    }

    void init()
    {
        m_loggingMiddleware.reset(new Middleware::LoggingMiddleware);
        m_databaseMiddleware.reset(new Middleware::DatabaseMiddleware);
        m_walletMiddleware.reset(new Middleware::WalletMiddleware);
        m_networkMiddleware.reset(new Middleware::NetworkMiddleware);
        m_taskMiddleware.reset(new Middleware::TaskMiddleware);
    }

    void deinit()
    {
        m_loggingMiddleware.clear();
        m_databaseMiddleware.clear();
        m_walletMiddleware.clear();
        m_networkMiddleware.clear();
        m_taskMiddleware.clear();
    }

    const auto& loggingMiddleware() const { return m_loggingMiddleware; }
    const auto& databaseMiddleware() const { return m_databaseMiddleware; }
    const auto& walletMiddleware() const { return m_walletMiddleware; }
    const auto& networkMiddleware() const { return m_networkMiddleware; }
    const auto& taskMiddleware() const { return m_taskMiddleware; }

    const auto& state() const { return m_state; }

signals:
    void stateChanged(const Data::State& state);

private:
    QSharedPointer<Middleware::LoggingMiddleware>  m_loggingMiddleware;
    QSharedPointer<Middleware::DatabaseMiddleware> m_databaseMiddleware;
    QSharedPointer<Middleware::WalletMiddleware>   m_walletMiddleware;
    QSharedPointer<Middleware::NetworkMiddleware>  m_networkMiddleware;
    QSharedPointer<Middleware::TaskMiddleware>     m_taskMiddleware;

    Data::State m_state;
};

Store& Store::instance()
{
    static Store instance;
    return instance;
}

Store::Store(QObject* parent)
    : m_thread(new QThread)
    , m_worker(new Worker)
{
    m_thread->setObjectName("XWallet::Store");

    m_worker.reset(new Worker);
    m_worker->moveToThread(m_thread.data());

    connect(m_thread.data(), &QThread::started, m_worker.data(), &Worker::init);
    connect(m_thread.data(), &QThread::finished, m_worker.data(), &Worker::deinit);

    connect(this, &Store::actionAdded, m_worker.data(), &Worker::applyAction, Qt::QueuedConnection);

    connect(m_worker.data(), &Worker::stateChanged, this, &Store::stateChanged);
    connect(m_thread.data(), &QThread::started, this, &Store::launched);

    m_thread->start();
}

Store::~Store()
{
    m_thread->quit();
    m_thread->wait();
}

QFuture<void> Store::dispatch(const RootActions::Action& action)
{
    const auto& promise = QSharedPointer<QPromise<void>>::create();

    emit actionAdded(action, promise);

    return promise->future();
}

Data::State Store::state() const
{
    return m_worker->state();
}

QSharedPointer<Middleware::LoggingMiddleware> Store::loggingMiddleware() const
{
    return m_worker->loggingMiddleware();
}

QSharedPointer<Middleware::DatabaseMiddleware> Store::databaseMiddleware() const
{
    return m_worker->databaseMiddleware();
}

QSharedPointer<Middleware::WalletMiddleware> Store::walletMiddleware() const
{
    return m_worker->walletMiddleware();
}

QSharedPointer<Middleware::NetworkMiddleware> Store::networkMiddleware() const
{
    return m_worker->networkMiddleware();
}

QSharedPointer<Middleware::TaskMiddleware> Store::taskMiddleware() const
{
    return m_worker->taskMiddleware();
}

} // namespace XWallet

#include "Store.moc"
