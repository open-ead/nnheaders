#pragma once

#include <nn/os.h>
#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/thread/atk_ThreadInfoReader.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/util/atk_Global.h>
#include <nn/atk/util/atk_AudioRendererPerformanceReader.h>
#include <nn/atk/util/atk_ProfileReader.h>

namespace nn::atk::detail::driver {
struct SoundThreadLock {};
struct AtkStateAndParameterUpdateLock {};

class SoundThread : fnd::Thread::Handler {
public:
    enum Message {
        Message_HwCallback = 0x10000000,
        Message_Shutdown = 0x20000000,
        Message_ForceWakeup = 0x30000000,
    };

    using ProfileFunc = void(*)(os::Tick*);

    constexpr static u32 ThreadMessageBuffferSize = 32;
    constexpr static u64 RendererEventWaitTimeoutMilliSeconds = 100;

    class SoundFrameCallback {
    public:
        virtual ~SoundFrameCallback();

        virtual void OnBeginSoundFrame();
        virtual void OnEndSoundFrame();

    private:
        friend SoundThread;

        util::IntrusiveListNode m_Link;
    };

    using SoundFrameCallbackList = util::IntrusiveList<SoundFrameCallback, 
                                util::IntrusiveListMemberNodeTraits<SoundFrameCallback, 
                                    &SoundFrameCallback::m_Link>>;

    class PlayerCallback {
    public:
        virtual ~PlayerCallback();

        virtual void OnUpdateFrameSoundThread() = 0;
        virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency() = 0;
        virtual void OnShutdownSoundThread() = 0;

    private:
        friend SoundThread;

        util::IntrusiveListNode m_Link;
    };

    using PlayerCallbackList = util::IntrusiveList<PlayerCallback, 
                                util::IntrusiveListMemberNodeTraits<PlayerCallback, 
                                    &PlayerCallback::m_Link>>;

    ~SoundThread() override;
    
    bool CreateSoundThread(s32 threadPriority, void* stackBase, std::size_t stackSize, 
                           s32 idealCoreNumber, u32 affinityMask);

    void Initialize(void* performanceFrameBuffer, std::size_t performanceFrameBufferSize, 
                    bool isProfilingEnabled);
    void Initialize(void* performanceFrameBuffer, std::size_t performanceFrameBufferSize, 
                    bool isProfilingEnabled, bool isDetailSoundThreadProfilerEnabled,
                    bool isUserThreadRenderingEnabled);

    void Destroy();
    
    void Finalize();

    void UpdateLowLevelVoices();

    void ForceWakeup();

    void RegisterSoundFrameUserCallback(SoundFrameUserCallback callback, std::uintptr_t callbackArg);
    void ClearSoundFrameUserCallback();

    void RegisterThreadBeginUserCallback(SoundThreadUserCallback, std::uintptr_t callbackArg);
    void ClearThreadBeginUserCallback();

    void RegisterThreadEndUserCallback(SoundThreadUserCallback, std::uintptr_t callbackArg);
    void ClearThreadEndUserCallback();

    void RegisterSoundFrameCallback(SoundFrameCallback* callback);
    void UnregisterSoundFrameCallback(SoundFrameCallback* callback);

    void RegisterPlayerCallback(PlayerCallback* callback);
    void UnregisterPlayerCallback(PlayerCallback* callback);

    void LockAtkStateAndParameterUpdate();
    void UnlockAtkStateAndParameterUpdate();

    void RegisterAudioRendererPerformanceReader(AudioRendererPerformanceReader& performanceReader);
    
    void RegisterSoundThreadUpdateProfileReader(SoundThreadUpdateProfileReader& profileReader);
    void UnregisterSoundThreadUpdateProfileReader(SoundThreadUpdateProfileReader& profileReader);

    void FrameProcess(UpdateType updateType);

    void RecordPerformanceInfo(audio::PerformanceInfo* src, os::Tick beginTick, 
                               os::Tick endTick, u32 nwVoiceCount);

    void EffectFrameProcess();

    void RecordUpdateProfile(const SoundThreadUpdateProfile& threadUpdateProfile);

    u32 Run(void* param) override;

private:
    fnd::Thread m_Thread;
    os::MessageQueue m_BlockingQueue;
    std::uintptr_t m_MsgBuffer[32];
    u32 m_AxCallbackCounter;
    fnd::CriticalSection m_CriticalSection;
    fnd::CriticalSection m_UpdateAtkStateAndParameterSection;
    SoundFrameCallbackList m_SoundFrameCallbackList;
    PlayerCallbackList m_PlayerCallbackList;
    SoundFrameUserCallback m_UserCallback;
    std::uintptr_t m_UserCallbackArg;
    SoundThreadUserCallback m_ThreadBeginUserCallback;
    std::uintptr_t m_ThreadBeginUserCallbackArg;
    SoundThreadUserCallback m_ThreadEndUserCallback;
    std::uintptr_t m_ThreadEndUserCallbackArg;
    s32 m_SoundThreadAffinityMask;
    bool m_CreateFlag;
    bool m_PauseFlag;
    os::Tick m_LastPerformanceFrameBegin;
    os::Tick m_LastPerformanceFrameEnd;
    void* m_pPerformanceFrameUpdateBuffer[3];
    std::size_t m_PerformanceFrameUpdateBufferSize;
    s32 m_CurrentPerformanceFrameBufferIndex;
    bool m_IsProfilingEnabled;
    ProfileFunc m_pSoundThreadProfileFunc;
    bool m_IsUserThreadRenderingEnabled;
    ProfileReaderList m_ProfileReaderList;
    AudioRendererPerformanceReader* m_pAudioRendererPerformanceReader;
    SoundThreadInfoRecorderList m_InfoRecorderList;
    fnd::CriticalSection m_LockRecordInfo;
    SoundThreadUpdateProfile m_LastUpdateProfile;
    SoundThreadUpdateProfileReaderList m_UpdateProfileReaderList;
    fnd::CriticalSection m_LockUpdateProfile;
    std::atomic_int m_RendererEventWaitTimeMilliSeconds;
};
static_assert(sizeof(SoundThread) == 0x508);
} // namespace nn::atk::detail::driver