#pragma once

#include <nn/atk/detail/atk_BasicSoundPlayer.h>
#include <nn/atk/detail/voice/atk_Channel.h>
#include <nn/atk/detail/wsd/atk_WaveSoundFileReader.h>
#include <nn/atk/detail/wsd/atk_WaveSoundLoader.h>

namespace nn::atk::detail {
struct WaveSoundDataInfo {
    bool loopFlag;
    s32 sampleRate;
    s64 loopStart;
    s64 loopEnd;
    s64 compatibleLoopStart;
    s64 compatibleLoopEnd;
    s32 channelCount;
};
static_assert(sizeof(WaveSoundDataInfo) == 0x30);

namespace driver { 
class WaveSoundPlayer : BasicSoundPlayer, DisposeCallback, SoundThread::PlayerCallback {
public:
    enum StartOffsetType {
        StartOffsetType_Sample,
        StartOffsetType_Millisec,
    };

    enum ResState {
        ResState_Invalid,
        ResState_ReceiveLoadReq,
        ResState_AppendLoadTask,
        ResState_Assigned,
    };

    constexpr static s32 SignatureFile = 0x44535746; // FWSD
    
    constexpr static u32 PauseReleaseValue = 127;
    constexpr static u32 MuteReleaseValue = 127;
    constexpr static u32 DefaultPriority = 64;

    struct StartInfo {
        s32 index;
        StartOffsetType startOffsetType;
        s32 startOffset;
        s32 delayTime;
        s32 delayCount;
        s32 waveSoundParameterFlag;
        s32 release;
        bool isContextCalculationSkipMode;
        UpdateType updateType;
    };
    static_assert(sizeof(StartInfo) == 0x24);

    struct PrepareArg {
        void* wsdFile;
        void* waveFile;
        s8 waveType;
        u8 padding[3];
    };
    static_assert(sizeof(PrepareArg) == 0x18);

    WaveSoundPlayer();
    ~WaveSoundPlayer() override;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void FinishPlayer();

    void CloseChannel();

    void FreeLoader();

    void Prepare(const StartInfo& info, const PrepareArg& arg);
    void PrepareForPlayerHeap(const PrepareArg& arg);

    void RequestLoad(const StartInfo& info, const WaveSoundLoader::Arg& arg);

    void Start() override;
    void Stop() override;
    void Pause(bool flag) override;

    void SetPanRange(f32 range);
    void SetChannelPriority(s32 priority);
    void SetReleasePriorityFix(bool fix);

    void InvalidateData(const void* start, const void* end) override;

    position_t GetPlaySamplePosition(bool) const;

    void Update();

    bool TryAllocLoader();

    bool StartChannel();
    void UpdateChannel();

    static void ChannelCallbackFunc(Channel* dropChannel, 
                                    Channel::ChannelCallbackStatus status, void* userData);

    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

private:
    bool m_WavePlayFlag;
    bool m_ReleasePriorityFixFlag;
    u8 m_Priority;
    s8 m_WaveType;
    f32 m_PanRange;
    void* m_pWsdFile;
    void* m_pWaveFile;
    s32 m_WaveSoundIndex;
    StartOffsetType m_StartOffsetType;
    position_t m_StartOffset;
    s32 m_DelayCount;
    s32 m_Release;
    s32 m_WaveSoundParameterFlag;
    bool m_IsContextCalculationSkipMode;
    CurveLfoParam m_LfoParam;
    WaveSoundInfo m_WaveSoundInfo;
    Channel* m_pChannel;
    UpdateType m_UpdateType;
    u8 m_ResState;
    bool m_IsInitialized;
    bool m_IsRegisterPlayerCallback;
    u8 m_Padding[1];
    WaveSoundLoaderManager* m_pLoaderManager;
    WaveSoundLoader* m_pLoader;
    WaveSoundLoader::Arg m_LoaderArg;
};
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(WaveSoundPlayer) == 0x1a0);
#else
static_assert(sizeof(WaveSoundPlayer) == 0x190);
#endif
} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail