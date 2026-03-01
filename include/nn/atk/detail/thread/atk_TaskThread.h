#pragma once

#include <nn/atk/atk_Global.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>

namespace nn::atk::detail {
class TaskThread : fnd::Thread::Handler {
public:
    TaskThread();
    ~TaskThread() override;

    void Destroy();

    bool Create(s32 priority, void* stackBase, size_t stackSize, 
                s32 idealCoreNumber, u32 affinityMask, FsPriority fsPriority);
    
    u32 Run(void* param) override;

private:
    fnd::Thread m_Thread;
    fnd::CriticalSection m_CriticalSection;
    bool m_IsFinished;
    bool m_IsCreated;
    FsPriority m_FsPriority;
};
static_assert(sizeof(TaskThread) == 0x220);
} // namespace nn::atk::detail