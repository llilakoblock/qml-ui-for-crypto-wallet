#pragma once

#include "actions/RootActions.h"
#include "services/TasksService.h"

namespace XWallet {

class Worker;

namespace Middleware {

class TaskMiddleware : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TaskMiddleware)

public:
    TaskMiddleware(QObject* parent = nullptr);

private:
    RootActions::Action apply(const RootActions::Action& action);

private:
    QScopedPointer<TasksService> m_tasksService;

    friend class XWallet::Worker;
};

} // namespace Middleware
} // namespace XWallet
