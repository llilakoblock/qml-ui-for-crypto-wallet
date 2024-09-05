#include "TasksService.h"

#include "singletones/Store.h"
#include "misc/VariantCast.h"

namespace XWallet {

TasksService::TasksService(QObject* parent)
    : QObject(parent)
{
}

TasksService::~TasksService() = default;

int TasksService::addTask(const RootActions::AddTaskAction& task)
{
    if (task.immediate && task.type == RootActions::AddTaskAction::Type::OneShot)
    {
        Store::instance().dispatch(variant_cast(task.action));
        return -1;
    }

    int timerId = startTimer(task.interval);

    m_tasks.insert(timerId, task);

    if (task.immediate)
    {
        Store::instance().dispatch(variant_cast(task.action));
    }

    return timerId;
}

void TasksService::timerEvent(QTimerEvent* event)
{
    if (!m_tasks.contains(event->timerId()))
    {
        killTimer(event->timerId());
    }

    const auto& task = m_tasks.value(event->timerId());

    Store::instance().dispatch(variant_cast(task.action));

    if (task.type == RootActions::AddTaskAction::Type::OneShot)
    {
        killTimer(event->timerId());
        m_tasks.remove(event->timerId());
    }
}

} // namespace XWallet
