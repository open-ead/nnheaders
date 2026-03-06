#pragma once

#include <nn/os.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_BasicSoundPlayer.h>
#include <nn/atk/detail/atk_MoveValue.h>
#include <nn/atk/detail/atk_OutputAdditionalParam.h>
#include <nn/atk/submix/atk_ChannelMixVolume.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk {
class SoundActor;
class SoundPlayer;
class SoundHandle;

class OutputAmbientParam {
private:
    f32 m_Volume;
    f32 m_Pan;
    f32 m_SurroundPan;
    f32 m_FxSend[3];
};
static_assert(sizeof(OutputAmbientParam) == 0x18);

class SoundAmbientParam {
public:
    constexpr static u32 OutputLineFlagInherit = -1;
    
private:
    f32 m_Volume;
    f32 m_Pitch;
    f32 m_Lpf;
    f32 m_BiquadFilterValue;
    s32 m_BiquadFilterType;
    s32 m_Priority;
    u32 m_UserData;
    s32 m_OutputLineFlag;
    OutputAmbientParam m_TvParam;
};
static_assert(sizeof(SoundAmbientParam) == 0x38);

class SoundParam {
private:
    f32 m_Volume;
    f32 m_Pitch;
    f32 m_Lpf;
    f32 m_BiquadFilterValue;
    s32 m_BiquadFilterType;
    s32 m_Priority;
    s32 m_OutputLineFlag;
    u32 m_UserData;
    OutputAmbientParam m_TvParam;
};
static_assert(sizeof(SoundParam) == 0x38);

struct SoundParamCalculationValues {
    struct SoundArchiveParam {
        f32 volume;
    };
    static_assert(sizeof(SoundArchiveParam) == 0x4);

    struct SoundPlayerParam {
        f32 volume;
        f32 lpf;
        s32 bqfType;
        f32 bqfValue;
        f32 outputVolume[1];
        f32 outputMainSend[1];
        f32 outputEffectSend[1][3];
    };
    
    struct Sound3DParam {
        f32 volume;
        f32 pitch;
        f32 lpf;
        s32 bqfType;
        f32 bqfValue;
        u32 outputLineFlag;
        f32 outputVolume[1];
        f32 outputPan[1];
        f32 outputSurroundPan[1];
        f32 outputEffectSend[1][3];
        s32 playerPriority;
    };

    struct SoundActorParam {
        f32 volume;
        f32 pitch;
        f32 lpf;
        f32 outputVolume[1];
        f32 outputPan[1];
    };

    struct SoundHandleParam {
        f32 volume;
        f32 pitch;
        f32 lpf;
        s32 bqfType;
        s32 bqfValue;
        u32 outputLineFlag;
        f32 outputVolume[1];
        f32 outputPan[1];
        f32 outputSurroundPan[1];
        f32 outputMainSend[1];
        f32 outputEffectSend[1][3];
        MixParameter outputMixParameter[1][2];
        MixMode mixMode;
        f32 pan;
        f32 surroundPan;
        f32 mainSend;
        f32 effectSend[3];
        s32 playerPriority;
    };

    struct ResultParam {
        f32 volume;
        f32 pitch;
        f32 lpf;
        s32 bqfType;
        f32 bqfValue;
        u32 outputLineFlag;
        detail::OutputParam outputParamResult[1];
        s32 playerPriority;
    };

    struct FadeVolumeParam {
        f32 stopFadeVolume;
        f32 pauseFadeVolume;
        f32 muteFadeVolume;
        bool isMuted;
    };

    SoundArchiveParam soundArchiveParam;
    SoundPlayerParam soundPlayerParam;
    Sound3DParam sound3DParam;
    SoundActorParam soundActorParam;
    SoundHandleParam soundHandleParam;
    ResultParam resultParam;
    FadeVolumeParam fadeVolumeParam;
};

namespace detail {
class PlayerHeap;
class ExternalSoundPlayer;

struct SoundActorParam {
    f32 volume;
    f32 pitch;
    f32 tvVolume;
    f32 tvPan;
    f32 lpf;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    s32 biquadFilterType;
    f32 biquadFilterValue;
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundActorParam) == 0x14);
#else
static_assert(sizeof(SoundActorParam) == 0x1c);
#endif

class BasicSound {
public:
    enum State {
        State_Constructed,
        State_Initialized,
        State_Finalized,
        State_Destructed,
    };

    enum MuteState {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting,
    };

    enum PauseState {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing,
    };

    enum PlayerState {
        PlayerState_Init,
        PlayerState_Play,
        PlayerState_Stop,
    };

    struct AmbientParamUpdateCallback{};
    struct AmbientArgUpdateCallback{};
    struct AmbientArgAllocatorCallback{};

    struct AmbientInfo {
        AmbientParamUpdateCallback* paramUpdateCallback;
        AmbientArgUpdateCallback* argUpdateCallback;
        AmbientArgAllocatorCallback* argAllocatorCallback;
        void* arg;
        u64 argSize;
    };
    static_assert(sizeof(AmbientInfo) == 0x28);

    struct CommonParam {
        detail::MoveValue<f32, s32> volume;
        MixMode mixMode;
        f32 pan;
        f32 span;
        f32 send[4];
    };
    static_assert(sizeof(CommonParam) == 0x2c);

    BasicSound();
    
    virtual ~BasicSound();
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual bool Initialize();
#else
    virtual bool Initialize(OutputReceiver* pOutputReceiver);
#endif
    virtual void Finalize();
    virtual bool IsPrepared() const = 0;
    virtual bool IsAttachedTempSpecialHandle() = 0;
    virtual void DetachTempSpecialHandle() = 0;
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() = 0;
    virtual void OnUpdatePlayerPriority();
    virtual void UpdateMoveValue();
    virtual void OnUpdateParam();

    void SetPriority(s32 priority, s32 ambientPriority);
    void GetPriority(s32* pPlayerAvailable, s32* pPriority) const;

    void ClearIsFinalizedForCannotAllocatedResourceFlag();

    void SetId(u32 id);

    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();

    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();

    void StartPrepared();
    void Stop(s32 fadeFrames);

    void SetPlayerPriority(s32 priority);

    void ForceStop();
    void Pause(bool, s32);
    void Pause(bool, s32, PauseMode pauseMode);
    
    void Mute(bool, s32);

    void SetAutoStopCounter(s32 autoStopCounter);

    void FadeIn(s32);

    bool IsPause() const;
    bool IsMute() const;

    void Update();
    void UpdateParam();

    f32 CalculateVolume() const;
    f32 CalculatePitch() const;
    f32 CalculateLpfFrequency() const;
    u32 CalculateOutLineFlag() const;
    void CalculateBiquadFilter(s32* pBiquadFilterType, f32* pBiquadFilterValue) const;
    void CalculateOutputParam(OutputParam*, OutputDevice) const;
    void CalculateOutputBusMixVolume(OutputBusMixVolume*, OutputDevice) const;

    bool IsVolumeThroughModeUsed() const;

    void ApplyCommonParam(OutputParam&) const;

    void AttachPlayerHeap(PlayerHeap* heap);
    void DetachPlayerHeap(PlayerHeap* heap);

    void AttachSoundPlayer(SoundPlayer* player);
    void DetachSoundPlayer(SoundPlayer* player);

    void AttachSoundActor(SoundActor* actor);
    void DetachSoundActor(SoundActor* actor);

    void AttachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);
    void DetachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);

    u32 GetRemainingFadeFrames() const;
    u32 GetRemainingPauseFadeFrames() const;
    u32 GetRemainingMuteFadeFrames() const;

    void CalculateSoundParamCalculationValues(SoundParamCalculationValues* pParamCalcValues) const;

    void SetInitialVolume(f32 volume);
    f32 GetInitialVolume() const;

    void SetVolume(f32 volume, s32 frames);
    s32 GetVolume() const;

    void SetPitch(f32 pitch);
    f32 GetPitch() const;

    void SetLpfFreq(f32 lpf);
    f32 GetLpfFreq() const;

    void SetBiquadFilter(s32 biquadFilterType, f32 biquadFilterValue);
    void GetBiquadFilter(s32*, f32*) const;

    void SetOutputLine(u32 outputLine);
    u32 GetOutputLine() const;
    void ResetOutputLine();

    void SetMixMode(MixMode mode);
    MixMode GetMixMode() const;

    void SetPan(f32 pan);
    f32 GetPan() const;

    void SetSurroundPan(f32 surroundPan);
    f32 GetSurroundPan() const;

    void SetMainSend(f32 mainSend);
    f32 GetMainSend() const;

    void SetFxSend(AuxBus auxBus, f32 fxSend);
    f32 GetFxSend(AuxBus auxBus) const;

    void SetSend(s32, f32 send);
    void SetOutputAdditionalSend(OutputDevice device, s32, f32 send);

    f32 GetSend(s32) const;
    void GetOutputAdditionalSend(OutputDevice device, s32) const;

    void SetVolumeThroughMode(s32, u8);
    void SetOutputVolumeThroughMode(OutputDevice device, s32, u8);

    void GetVolumeThroughMode(s32);
    void GetOutputVolumeThroughMode(OutputDevice device, s32) const;

    s32 GetSendBusCount();
    s32 GetSendChannelCount();

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputAdditionalParamAddr(OutputDevice device, OutputAdditionalParam* addr, 
                                      OutputAdditionalParam* addrForPlayer);
#endif

    void SetOutputVolume(OutputDevice device, f32 volume);
    void SetOutputBusMixVolume(OutputDevice, u32, u32, ChannelMixVolume);
    void SetOutputChannelMixParameter(OutputDevice, u32, MixParameter);
    void SetOutputPan(OutputDevice device, f32 pan);
    void SetOutputSurroundPan(OutputDevice device, f32 surroundPan);
    void SetOutputMainSend(OutputDevice device, f32 mainSend);
    void SetOutputFxSend(OutputDevice device, AuxBus auxBus, f32 fxSend);
    void SetOutputFxSend(OutputDevice device, f32 fxSend);
    void SetOutputBusMixVolumeEnabled(OutputDevice device, s32, bool);

    f32 GetOutputVolume(OutputDevice device) const;
    MixParameter GetOutputChannelMixParameter(OutputDevice, u32) const;
    f32 GetOutputPan(OutputDevice device) const;
    f32 GetOutputSurroundPan(OutputDevice device) const;
    f32 GetOutputMainSend(OutputDevice device) const;
    f32 GetOutputFxSend(OutputDevice device, AuxBus auxBus) const;
    f32 GetOutputFxSend(OutputDevice device) const;

    bool IsOutputBusMixVolumeEnabled(OutputDevice device, s32) const;
    void GetOutputBusMixVolume(OutputDevice device, s32, s32) const;

    void SetPanMode(PanMode mode);
    void SetPanCurve(PanCurve curve);
    void SetAmbientInfo(AmbientInfo* ambientArgInfo);
    
    static s32 GetAmbientPriority(const AmbientInfo& ambientInfo, u32 soundId);
    
    void SetSetupTick(const os::Tick& tick);
    void SetSoundArchive(const SoundArchive* soundArchive);

    PanMode GetPanMode() const;
    PanCurve GetPanCurve() const;
    AmbientInfo* GetAmbientInfo() const; 

private:
    friend SoundPlayer;
    friend ExternalSoundPlayer;

    PlayerHeap* m_pPlayerHeap;
    SoundHandle* m_pGeneralHandle;                            
    SoundHandle* m_pTempGeneralHandle;
    SoundPlayer* m_pSoundPlayer;
    SoundActor* m_pSoundActor;
    ExternalSoundPlayer* m_pExtSoundPlayer;
    SoundArchive* m_pSoundArchive;
    AmbientInfo m_AmbientInfo;
    SoundParam m_AmbientParam;
    SoundActorParam m_ActorParam;
    MoveValue<f32, s32> m_FadeVolume;
    MoveValue<f32, s32> m_PauseFadeVolume;
    MoveValue<f32, s32> m_MuteFadeVolume;
    bool m_StartFlag;
    bool m_StartedFlag;
    bool m_AutoStopFlag;
    bool m_FadeOutFlag;
    bool m_PlayerAvailableFlag;
    bool m_UnPauseFlag;
    PauseMode m_PauseMode;
    u8 m_Priority;
    s8 m_BiquadFilterType;
    State m_State;
    u8 m_PlayerState;
    u8 m_PauseState;
    u8 m_MuteState;
    u8 m_Padding[1];
    s32 m_AutoStopCounter;
    u32 m_UpdateCounter;
    u32 m_PlayingCounter;
    u32 m_Id;
    u32 m_InstanceId;
    os::Tick m_SetupTick;
    f32 m_InitVolume;
    f32 m_Pitch;
    f32 m_LpfFreq;
    f32 m_BiquadFilterValue;
    u32 m_OutputLineFlag;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pOutputReceiver;
#endif
    CommonParam m_CommonParam;
    OutputParam m_OutputParam[1];
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputAdditionalParam* m_pOutputAdditionalParam;
#endif
    void* m_pUserParam;
    std::size_t m_UserParamSize;
    SoundStopCallback m_SoundStopCallback;
    util::IntrusiveListNode m_SoundPlayerPlayLink;
    util::IntrusiveListNode m_SoundPlayerPriorityLink;
    util::IntrusiveListNode m_ExtSoundPlayerPlayLink;

    static s32 g_LastInstanceId;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(BasicSound) == 0x1f0);
#else
static_assert(sizeof(BasicSound) == 0x210);
#endif
}  // namespace detail
}  // namespace nn::atk
