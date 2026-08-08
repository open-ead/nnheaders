#pragma once

#include <nn/os.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_OutputAdditionalParam.h>
#include <nn/atk/atk_ChannelMixVolume.h>
#include <nn/atk/atk_OutputReceiver.h>

namespace nn::atk {
class SoundActor;
class SoundPlayer;
class SoundHandle;

class OutputAmbientParam {
public:
    OutputAmbientParam();

    void Initialize();

    float GetVolume() const;
    void SetVolume(float volume);

    float GetPan() const;
    void SetPan(float pan);

    float GetSurroundPan() const;
    void SetSurroundPan(float surroundPan);

    float GetEffectSend(int) const;
    void SetEffectSend(int, float) const;

private:
    f32 m_Volume;
    f32 m_Pan;
    f32 m_SurroundPan;
    f32 m_FxSend[3];
};
static_assert(sizeof(OutputAmbientParam) == 0x18);

class SoundAmbientParam {
public:
    static const int OutputLineFlagInherit = -1;
    
    SoundAmbientParam();

    float GetVolume() const { return m_Volume; }
    void SetVolume(float volume) { m_Volume = volume; }

    float GetPitch() const { return m_Pitch; }
    void SetPitch(float pitch) { m_Pitch = pitch; }

    float GetLowPassFilter() const { return m_Lpf; }
    void SetLowPassFilter(float lowPassFilter) { m_Lpf = lowPassFilter; }

    float GetBiquadFilterValue() const { return m_BiquadFilterValue; }
    void SetBiquadFilterValue(float biquadFilterValue) { m_BiquadFilterValue = biquadFilterValue; }

    int GetBiquadFilterType() const { return m_BiquadFilterType; }
    void SetBiquadFilterType(int biquadFilterType) { m_BiquadFilterType = biquadFilterType; }

    int GetPriority() const { return m_Priority; }
    void SetPriority(int priority) { m_Priority = priority; }

    u32 GetUserData() const { return m_UserData; }
    void SetUserData(u32 userData) { m_UserData = userData; }

    int GetOutputLineFlag() const { return m_OutputLineFlag; }
    void SetOutputLineFlag(int outputLineFlag) { m_OutputLineFlag = outputLineFlag; }

    OutputAmbientParam& GetTvParam() { return m_TvParam; }
    void SetTvParam(const OutputAmbientParam& tvParam) { m_TvParam = tvParam; }

private:
    float m_Volume;
    float m_Pitch;
    float m_Lpf;
    float m_BiquadFilterValue;
    int m_BiquadFilterType;
    int m_Priority;
    u32 m_UserData;
    int m_OutputLineFlag;
    OutputAmbientParam m_TvParam;
};
static_assert(sizeof(SoundAmbientParam) == 0x38);

class SoundParam {
public:
    SoundParam();

    void Initialize();

    float GetVolume() const { return m_Volume; }
    void SetVolume(float volume) { m_Volume = volume; }

    float GetPitch() const { return m_Pitch; }
    void SetPitch(float pitch) { m_Pitch = pitch; }

    float GetLpf() const { return m_Lpf; }
    void SetLpf(float lpf) { m_Lpf = lpf; }

    float GetBiquadFilterValue() const { return m_BiquadFilterValue; }
    void SetBiquadFilterValue(float biquadFilterValue) { m_BiquadFilterValue = biquadFilterValue; }

    int GetBiquadFilterType() const { return m_BiquadFilterType; }
    void SetBiquadFilterType(int biquadFilterType) { m_BiquadFilterType = biquadFilterType; }

    int GetPriority() const { return m_Priority; }
    void SetPriority(int priority) { m_Priority = priority; }

    int GetOutputLineFlag() const { return m_OutputLineFlag; }
    void SetOutputLineFlag(int outputLineFlag) { m_OutputLineFlag = outputLineFlag; }

    u32 GetUserData() const { return m_UserData; }
    void SetUserData(u32 userData) { m_UserData = userData; }

    OutputAmbientParam& GetTvParam() { return m_TvParam; }
    const OutputAmbientParam& GetTvParam() const { return m_TvParam; }
    void SetTvParam(const OutputAmbientParam& tvParam) { m_TvParam = tvParam; }

private:
    float m_Volume;
    float m_Pitch;
    float m_Lpf;
    float m_BiquadFilterValue;
    int m_BiquadFilterType;
    int m_Priority;
    int m_OutputLineFlag;
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
        float volume;
        float lpf;
        int bqfType;
        float bqfValue;
        float outputVolume[1];
        float outputMainSend[1];
        float outputEffectSend[1][3];
    };
    static_assert(sizeof(SoundPlayerParam) == 0x24);
    
    struct Sound3DParam {
        float volume;
        float pitch;
        float lpf;
        int bqfType;
        float bqfValue;
        u32 outputLineFlag;
        float outputVolume[1];
        float outputPan[1];
        float outputSurroundPan[1];
        float outputEffectSend[1][3];
        int playerPriority;
    };
    static_assert(sizeof(Sound3DParam) == 0x34);

    struct SoundActorParam {
        float volume;
        float pitch;
        float lpf;
        float outputVolume[1];
        float outputPan[1];
    };
    static_assert(sizeof(SoundActorParam) == 0x14);

    struct SoundHandleParam {
        float volume;
        float pitch;
        float lpf;
        int bqfType;
        int bqfValue;
        u32 outputLineFlag;
        float outputVolume[1];
        float outputPan[1];
        float outputSurroundPan[1];
        float outputMainSend[1];
        float outputEffectSend[1][3];
        MixParameter outputMixParameter[1][2];
        MixMode mixMode;
        float pan;
        float surroundPan;
        float mainSend;
        float effectSend[3];
        int playerPriority;
    };
    static_assert(sizeof(SoundHandleParam) == 0x84);

    struct ResultParam {
        float volume;
        float pitch;
        float lpf;
        int bqfType;
        float bqfValue;
        u32 outputLineFlag;
        detail::OutputParam outputParamResult[1];
        int playerPriority;
    };
    static_assert(sizeof(ResultParam) == 0x6c);

    struct FadeVolumeParam {
        float stopFadeVolume;
        float pauseFadeVolume;
        float muteFadeVolume;
        bool isMuted;
    };
    static_assert(sizeof(FadeVolumeParam) == 0x10);

    SoundArchiveParam soundArchiveParam;
    SoundPlayerParam soundPlayerParam;
    Sound3DParam sound3DParam;
    SoundActorParam soundActorParam;
    SoundHandleParam soundHandleParam;
    ResultParam resultParam;
    FadeVolumeParam fadeVolumeParam;
};
static_assert(sizeof(SoundParamCalculationValues) == 0x170);

namespace detail {
class PlayerHeap;
class ExternalSoundPlayer;

struct SoundActorParam {
    float volume;
    float pitch;
    float tvVolume;
    float tvPan;
    float lpf;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    int biquadFilterType;
    float biquadFilterValue;
#endif

    SoundActorParam();

    void Reset();
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundActorParam) == 0x14);
#else
static_assert(sizeof(SoundActorParam) == 0x1c);
#endif

class BasicSound {
// TODO: implement rtti in atk::fnd
public:
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

    BasicSound();
    virtual ~BasicSound();

    void Update();
    void StartPrepared();

    void Stop(int fadeFrames);
    void ForceStop();

    void Pause(bool flag, int fadeFrames);
    void Pause(bool flag, int fadeFrames, PauseMode pauseMode);
    void Mute(bool flag, int fadeFrames);

    void SetAutoStopCounter(int frames);
    void FadeIn(int frames);

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual bool Initialize();
#else
    virtual bool Initialize(OutputReceiver* pOutputReceiver);
#endif
    virtual void Finalize();

    virtual bool IsPrepared() const = 0;
    bool IsPause() const;
    bool IsMute() const;
    bool IsStarted() const;

    void SetPriority(int priority, int ambientPriority);
    void GetPriority(int* priority, int* ambientPriority) const;

    void SetInitialVolume(float volume);
    float GetInitialVolume() const;

    void SetVolume(float volume, int frames);
    float GetVolume() const;

    void SetPitch(float pitch);
    float GetPitch() const;

    void SetLpfFreq(float lpfFreq);
    float GetLpfFreq() const;

    void SetBiquadFilter(int type, float value);
    void GetBiquadFilter(int* type, float* value) const;

    void SetOutputLine(u32 lineFlag);
    u32 GetOutputLine() const;
    void ResetOutputLine();

    void SetPlayerPriority(int priority);
    void SetMixMode(MixMode mixMode);
    MixMode GetMixMode();

    void SetPan(float pan);
    float GetPan() const;

    void SetSurroundPan(float span);
    float GetSurroundPan() const;

    void SetMainSend(float send);
    float GetMainSend() const;

    void SetFxSend(AuxBus bus, float send);
    float GetFxSend(AuxBus bus) const;

    void SetSend(int bus, float send);
    float GetSend(int bus) const;

    void SetVolumeThroughMode(int bus, u8 modeBitFlag);
    u8 GetVolumeThroughMode(int bus);
    bool IsVolumeThroughModeUsed() const;

    int GetSendBusCount();
    int GetSendChannelCount();

    void SetPanMode(PanMode mode);
    void SetPanCurve(PanCurve curve);
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputAdditionalParamAddr(OutputDevice device, OutputAdditionalParam* addr, 
                                      OutputAdditionalParam* addrForPlayer);
#endif
    void SetOutputVolume(OutputDevice device, float volume);
    void SetOutputPan(OutputDevice device, float pan);
    void SetOutputSurroundPan(OutputDevice device, float span);
    void SetOutputMainSend(OutputDevice device, float send);
    void SetOutputFxSend(OutputDevice device, AuxBus bus, float send);
    void SetOutputFxSend(OutputDevice device, float send);
    void SetOutputAdditionalSend(OutputDevice device, int bus, float send);
    void SetOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus, bool isEnabled);
    void SetOutputBusMixVolume(OutputDevice device, int srcChNo, int subMixBus, ChannelMixVolume param);
    void SetOutputChannelMixParameter(OutputDevice device, u32 srcChNo, MixParameter param);
    void SetOutputVolumeThroughMode(OutputDevice device, int bus, u8 modeBitFlag);

    float GetOutputVolume(OutputDevice device) const;
    float GetOutputPan(OutputDevice device) const;
    float GetOutputSurroundPan(OutputDevice device) const;
    float GetOutputMainSend(OutputDevice device) const;
    float GetOutputFxSend(OutputDevice device, AuxBus bus) const;
    float GetOutputFxSend(OutputDevice device) const;
    bool IsOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus) const;
    ChannelMixVolume GetOutputBusMixVolume(OutputDevice device, int srcChNo, int subMixBus) const;
    float GetOutputAdditionalSend(OutputDevice device, int bus) const;
    u8 GetOutputVolumeThroughMode(OutputDevice device, int bus) const;
    MixParameter GetOutputChannelMixParameter(OutputDevice, u32 srcChNo) const;
    int GetRemainingFadeFrames() const;
    int GetRemainingPauseFadeFrames() const;
    int GetRemainingMuteFadeFrames() const;
    int GetPlayerPriority() const;

    void CalculateSoundParamCalculationValues(SoundParamCalculationValues* pOutValue) const;

    void SetId(u32 id);
    u32 GetId() const;
    u32 GetInstanceId() const;
    u32 GetPlayFrameCount() const;

    void SetSetupTick(const os::Tick& tick);
    os::Tick GetSetupTick() const;

    int CalcCurrentPlayerPriority() const;

    SoundPlayer* GetSoundPlayer();
    const SoundPlayer* GetSoundPlayer() const;
    void AttachSoundPlayer(SoundPlayer* player);
    void DetachSoundPlayer(SoundPlayer* player);

    const SoundActor* GetSoundActor() const;
    void AttachSoundActor(SoundActor* actor);
    void DetachSoundActor(SoundActor* actor);

    void AttachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);
    void DetachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);

    void AttachPlayerHeap(PlayerHeap* pHeap);
    void DetachPlayerHeap(PlayerHeap* pHeap);
    PlayerHeap* GetPlayerHeap();

    void SetAmbientInfo(const AmbientInfo& ambientArgInfo);
    void ClearAmbientArgUpdateCallback();
    void ClearAmbientParamUpdateCallback();
    void ClearAmbientArgAllocatorCallback();
    const SoundParam& GetAmbientParam() const;
    static int GetAmbientPriority(const AmbientInfo& ambientInfo, u32 soundId);

    bool IsAttachedGeneralHandle();
    bool IsAttachedTempGeneralHandle();
    virtual bool IsAttachedTempSpecialHandle() = 0;

    void DetachGeneralHandle();
    void DetachTempGeneralHandle();
    virtual void DetachTempSpecialHandle() = 0;

    void SetUserParamBuffer(void* buffer, size_t size);
    void* GetUserParam();

    void SetSoundStopCallback(const SoundStopCallback callback);
    void SetSoundArchive(const SoundArchive* soundArchive);
    const SoundArchive* GetSoundArchive() const;

    enum PauseState {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing,
    };
    PauseState GetPauseState() const;

    enum MuteState {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting,
    };
    MuteState GetMuteState() const;

protected:
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() = 0;
    virtual void OnUpdatePlayerPriority();
    virtual void UpdateMoveValue();
    virtual void OnUpdateParam();

    bool IsPlayerAvailable() const;

private:
    void UpdateParam();

    void ClearIsFinalizedForCannotAllocatedResourceFlag();

    float CalculateVolume() const;
    float CalculatePitch() const;
    float CalculateLpfFrequency() const;
    void CalculateBiquadFilter(int* pOutBiquadType, float* pOutBiquadValue) const;
    u32 CalculateOutLineFlag() const;
    void CalculateOutputParam(OutputParam* pOutParam, OutputDevice device) const;
    void CalculateOutputBusMixVolume(OutputBusMixVolume* pOutParam, OutputDevice device) const;
    
    void ApplyCommonParam(OutputParam& param) const;

    enum State {
        State_Constructed,
        State_Initialized,
        State_Finalized,
        State_Destructed,
    };

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
    MoveValue<float, int> m_FadeVolume;
    MoveValue<float, int> m_PauseFadeVolume;
    MoveValue<float, int> m_MuteFadeVolume;
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

    struct CommonParam {
        MoveValue<float, int> volume;
        MixMode mixMode;
        float pan;
        float span;
        float send[4];

        void Initialize();
        void Update();

        float GetVolume() const;
        void SetVolume(float target, int frame);

        CommonParam();
    };
    static_assert(sizeof(CommonParam) == 0x2c);

    CommonParam m_CommonParam;

    OutputParam m_OutputParam[1];
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputAdditionalParam* m_pOutputAdditionalParam[1];
#endif
    void* m_pUserParam;
    size_t m_UserParamSize;
    SoundStopCallback m_SoundStopCallback;

    static u32 g_LastInstanceId;

public:
    util::IntrusiveListNode m_SoundPlayerPlayLink;
    util::IntrusiveListNode m_SoundPlayerPriorityLink;
    util::IntrusiveListNode m_ExtSoundPlayerPlayLink;

};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(BasicSound) == 0x1f0);
#else
static_assert(sizeof(BasicSound) == 0x210);
#endif
}  // namespace detail
}  // namespace nn::atk
