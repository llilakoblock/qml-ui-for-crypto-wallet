#include "TaskMiddleware.h"

namespace XWallet::Middleware {

TaskMiddleware::TaskMiddleware(QObject* parent)
    : QObject(parent)
    , m_tasksService(new TasksService)
{
}

RootActions::Action TaskMiddleware::apply(const RootActions::Action& action)
{
    if (std::holds_alternative<RootActions::AddTaskAction>(action))
    {
        m_tasksService->addTask(std::get<RootActions::AddTaskAction>(action));

        return RootActions::EmptyAction {};
    }

    return action;
}

} // namespace XWallet::Middleware
