#pragma once

#include "actions/RootActions.h"

namespace XWallet {

class Worker;

namespace Middleware {

class LoggingMiddleware : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(LoggingMiddleware)

public:
    explicit LoggingMiddleware(QObject* parent = nullptr);

private:
    RootActions::Action apply(const RootActions::Action& action);

    friend class XWallet::Worker;
};

} // namespace Middleware
} // namespace XWallet
