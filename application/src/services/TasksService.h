#pragma once

#include "actions/RootActions.h"

#include <QObject>
#include <QSharedPointer>

namespace XWallet {

class TasksService final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TasksService)

public:
    enum class TaskType
    {
        OneShot,
        Interval,
    };
    Q_ENUM(TaskType)

public:
    explicit TasksService(QObject* parent = nullptr);
    ~TasksService() override;

    int addTask(const RootActions::AddTaskAction& task);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    QHash<int, RootActions::AddTaskAction> m_tasks;
};

} // namespace XWallet
