#pragma once

#include <nn/os/os_MessageQueue.h>

#include <nn/atk/detail/thread/atk_Task.h>

namespace nn::atk::detail {
class TaskManager {
public:
    using TaskList = util::IntrusiveList<Task, util::IntrusiveListMemberNodeTraits<Task, &Task::m_TaskLink>>;

    enum Message {
        Message_Append,
    };

    enum TaskPriority {
        TaskPriority_Low,
        TaskPriority_Middle,
        TaskPriority_High,
    };

    TaskManager();
    ~TaskManager();

    void Initialize(bool isEnableProfiling);
    void Finalize();

    void AppendTask(Task* task, TaskPriority priority);

    Task* GetNextTask(TaskPriority priority, bool);

    Task* PopTask();
    Task* PeekTask();

    void ExecuteTask();
    void CancelTask(Task* task);

    bool TryRemoveTask(Task* task);

    void CancelTaskById(u32 id);
    void RemoveTaskById(u32 id);

    void CancelAllTask();
    
    void WaitTask();
    void CancelWaitTask();

private:
    TaskList m_TaskList[3];
    bool m_IsWaitTaskCancel;
    fnd::CriticalSection m_CriticalSection;
    os::MessageQueue m_BlockingQueue;
    std::uintptr_t m_MsgBuffer[32];
    TaskProfileLogger m_TaskProfileLogger;
};
static_assert(sizeof(TaskManager) == 0x1d8);
} // namespace nn::atk::detail