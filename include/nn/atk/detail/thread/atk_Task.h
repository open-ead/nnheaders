#pragma once

#include <nn/atk/util/atk_TaskProfileReader.h>

namespace nn::atk::detail {
class TaskManager;

class Task {
public:
    enum Status {
        Status_Free,
        Status_Append,
        Status_Execute,
        Status_Done,
        Status_Cancel,
    };

    Task();
    
    virtual ~Task();
    virtual void Execute(TaskProfileLogger& logger) = 0;

private:
    friend TaskManager;

    util::IntrusiveListNode m_TaskLink;
    os::Event m_Event;
    Status m_Status;
    u32 m_Id;
};
static_assert(sizeof(Task) == 0x48);
} // namespace nn::atk::detail