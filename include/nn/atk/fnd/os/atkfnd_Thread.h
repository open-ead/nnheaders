#pragma once

#include <nn/types.h>
#include <nn/os/os_ThreadTypes.h>

namespace nn::atk::detail::fnd {
class TimeSpan;

class Thread {
public:
    enum AffinityMask {
        AffinityMask_CoreAll = -1,
        AffinityMask_CoreDefault,
        AffinityMask_Core0,
        AffinityMask_Core1  = AffinityMask_Core0 * 2,
        AffinityMask_Core2  = AffinityMask_Core1 * 2,
        AffinityMask_Core3  = AffinityMask_Core2 * 2,
        AffinityMask_Core4  = AffinityMask_Core3 * 2,
        AffinityMask_Core5  = AffinityMask_Core4 * 2,
        AffinityMask_Core6  = AffinityMask_Core5 * 2,
        AffinityMask_Core7  = AffinityMask_Core6 * 2,
        AffinityMask_Core8  = AffinityMask_Core7 * 2,
        AffinityMask_Core9  = AffinityMask_Core8 * 2,
        AffinityMask_Core10 = AffinityMask_Core9 * 2,
        AffinityMask_Core11 = AffinityMask_Core10 * 2,
        AffinityMask_Core12 = AffinityMask_Core11 * 2,
        AffinityMask_Core13 = AffinityMask_Core12 * 2,
        AffinityMask_Core14 = AffinityMask_Core13 * 2,
        AffinityMask_Core15 = AffinityMask_Core14 * 2,
        AffinityMask_Core16 = AffinityMask_Core15 * 2,
        AffinityMask_Core17 = AffinityMask_Core16 * 2,
        AffinityMask_Core18 = AffinityMask_Core17 * 2,
        AffinityMask_Core19 = AffinityMask_Core18 * 2,
        AffinityMask_Core20 = AffinityMask_Core19 * 2,
        AffinityMask_Core21 = AffinityMask_Core20 * 2,
        AffinityMask_Core22 = AffinityMask_Core21 * 2,
        AffinityMask_Core23 = AffinityMask_Core22 * 2,
        AffinityMask_Core24 = AffinityMask_Core23 * 2,
        AffinityMask_Core25 = AffinityMask_Core24 * 2,
        AffinityMask_Core26 = AffinityMask_Core25 * 2,
        AffinityMask_Core27 = AffinityMask_Core26 * 2,
        AffinityMask_Core28 = AffinityMask_Core27 * 2,
        AffinityMask_Core29 = AffinityMask_Core28 * 2,
        AffinityMask_Core30 = AffinityMask_Core29 * 2,
        AffinityMask_Core31 = -AffinityMask_Core30 * 2,
    };

    enum FsPriority {
        FsPriority_RealTime,
        FsPriority_Normal,
        FsPriority_Low,
    };

    enum State {
        State_NotRun,
        State_Running,
        State_Exited,
        State_Released,
    };

    using Handle = os::ThreadType;

    class Handler {
    public:
        virtual ~Handler() = 0;
        virtual u32 Run(void* param) = 0;
    };
    static_assert(sizeof(Handler) == 0x8);

    struct RunArgs {
        RunArgs();

        bool IsValid() const;

        char* name;
        void* stack;
        std::size_t stackSize;
        s32 idealCoreNumber;
        AffinityMask affinityMask;
        s32 priority;
        FsPriority fsPriority;
        void* param;
        Handler* handler;
    };
    static_assert(sizeof(RunArgs) == 0x38);

    ~Thread();

    bool Run(const RunArgs& args);
    void WaitForExit();
    void Release();

    void SetState(State state);

    s32 GetPriority() const;
    State GetState() const;

    void OnRun();
    void OnExit();

    Thread();

    void SetPriority(s32 priority);

    static void Sleep(const TimeSpan& timeSpan);

    bool Create(const Handle& handle, const s64& id, const RunArgs& args);

    void Detach();

    bool IsTerminated() const;

    void SetName(const char* name);
    void SetAffinityMask(s32 idealCoreNumber, AffinityMask value);

    void Resume();
    void Join();

private:
    u32 m_State;
    Handle m_Handle;
    s64 m_Id;
    s32 m_Priority;
    FsPriority m_FsPriority;
    void* m_Param;
    Handler* m_Handler;
    bool m_IsTerminated;
};
static_assert(sizeof(Thread) == 0x1f0);
} // nn::atk::detail::fnd