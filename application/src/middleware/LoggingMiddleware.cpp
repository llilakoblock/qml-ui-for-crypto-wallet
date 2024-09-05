#include "LoggingMiddleware.h"

#include "misc/Streamer.h"

Q_DECLARE_LOGGING_CATEGORY(store)
Q_LOGGING_CATEGORY(store, "store")

namespace XWallet::Middleware {

LoggingMiddleware::LoggingMiddleware(QObject* parent)
    : QObject(parent)
{}

RootActions::Action LoggingMiddleware::apply(const RootActions::Action& action)
{
    //qCDebug(store) << Streamer(action);

    return action;
}

} // namespace XWallet::Middleware
