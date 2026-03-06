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
        AffinityMask_Core0  = 1 <<  0,
        AffinityMask_Core1  = 1 <<  1,
        AffinityMask_Core2  = 1 <<  2,
        AffinityMask_Core3  = 1 <<  3,
        AffinityMask_Core4  = 1 <<  4,
        AffinityMask_Core5  = 1 <<  5,
        AffinityMask_Core6  = 1 <<  6,
        AffinityMask_Core7  = 1 <<  7,
        AffinityMask_Core8  = 1 <<  8,
        AffinityMask_Core9  = 1 <<  9,
        AffinityMask_Core10 = 1 << 10,
        AffinityMask_Core11 = 1 << 11,
        AffinityMask_Core12 = 1 << 12,
        AffinityMask_Core13 = 1 << 13,
        AffinityMask_Core14 = 1 << 14,
        AffinityMask_Core15 = 1 << 15,
        AffinityMask_Core16 = 1 << 16,
        AffinityMask_Core17 = 1 << 17,
        AffinityMask_Core18 = 1 << 18,
        AffinityMask_Core19 = 1 << 19,
        AffinityMask_Core20 = 1 << 20,
        AffinityMask_Core21 = 1 << 21,
        AffinityMask_Core22 = 1 << 22,
        AffinityMask_Core23 = 1 << 23,
        AffinityMask_Core24 = 1 << 24,
        AffinityMask_Core25 = 1 << 25,
        AffinityMask_Core26 = 1 << 26,
        AffinityMask_Core27 = 1 << 27,
        AffinityMask_Core28 = 1 << 28,
        AffinityMask_Core29 = 1 << 29,
        AffinityMask_Core30 = 1 << 30,
        AffinityMask_Core31 = 1 << 31,
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

    constexpr static u64 InvalidId = 0xFFFFFFFF;

    constexpr static u32 DefaultThreadPriority = 16;
    constexpr static u32 MinThreadPriority = 0;
    constexpr static u32 MaxThreadPriority = 31;

    constexpr static u32 StackAlignment = 4096;

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
        size_t stackSize;
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

    bool Create(const Handle& handle, s64& id, const RunArgs& args);

    void Detach();

    void SetName(const char* name);
    void SetAffinityMask(s32 idealCoreNumber, AffinityMask value);

    void Resume();
    void Join();

    bool IsTerminated() const;

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