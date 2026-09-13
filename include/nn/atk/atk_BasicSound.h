#pragma once

#include <nn/os.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_BasicSoundPlayer.h>
#include <nn/atk/atk_ChannelMixVolume.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/fnd/basis/atkfnd_Inlines.h>
#include <nn/atk/fnd/basis/atkfnd_RuntimeTypeInfo.h>

namespace nn::atk {

class SoundActor;
class SoundPlayer;
class SoundHandle;

class OutputAmbientParam {
public:
    OutputAmbientParam() = default;

    void Initialize() {
        m_Volume = 1.0f;
        m_Pan = 0.0f;
        m_SurroundPan = 0.0f;

        for (int i{0}; i < AuxBus_Count; ++i)
            m_FxSend[i] = 0.0f;
    }

    float GetVolume() const { return m_Volume; }
    void SetVolume(float volume) { m_Volume = volume; }

    float GetPan() const { return m_Pan; }
    void SetPan(float pan) { m_Pan = pan; }

    float GetSurroundPan() const { return m_SurroundPan; }
    void SetSurroundPan(float surroundPan) { m_SurroundPan = surroundPan; }

    float GetEffectSend(int auxBusIndex) const { return m_FxSend[auxBusIndex]; }
    void SetEffectSend(int auxBusIndex, float send) { m_FxSend[auxBusIndex] = send; }

private:
    f32 m_Volume;
    f32 m_Pan;
    f32 m_SurroundPan;
    f32 m_FxSend[AuxBus_Count];
};
static_assert(sizeof(OutputAmbientParam) == 0x18);

class SoundAmbientParam {
public:
    static const int OutputLineFlagInherit{-1};

    SoundAmbientParam() { m_TvParam.Initialize(); }

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
    float m_Volume{1.0f};
    float m_Pitch{1.0f};
    float m_Lpf{0.0f};
    float m_BiquadFilterValue{0.0f};
    int m_BiquadFilterType{BiquadFilterType_Inherit};
    int m_Priority{0};
    u32 m_UserData;
    int m_OutputLineFlag;
    OutputAmbientParam m_TvParam;
};
static_assert(sizeof(SoundAmbientParam) == 0x38);

class SoundParam {
public:
    SoundParam() { Initialize(); }

    void Initialize() {
        m_Volume = 1.0f;
        m_Pitch = 1.0f;
        m_Lpf = 0.0f;
        m_BiquadFilterType = BiquadFilterType_Inherit;
        m_BiquadFilterValue = 0.0f;
        m_Priority = 0;
        m_OutputLineFlag = SoundAmbientParam::OutputLineFlagInherit;
        m_UserData = 0;
        m_TvParam.Initialize();
    }

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
        float volume;
    };
    static_assert(sizeof(SoundArchiveParam) == 0x4);

    struct SoundPlayerParam {
        float volume;
        float lpf;
        int bqfType;
        float bqfValue;
        float outputVolume[1];
        float outputMainSend[1];
        float outputEffectSend[1][AuxBus_Count];
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
        float outputEffectSend[1][AuxBus_Count];
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
        float outputEffectSend[1][AuxBus_Count];
        MixParameter outputMixParameter[1][2];
        MixMode mixMode;
        float pan;
        float surroundPan;
        float mainSend;
        float effectSend[AuxBus_Count];
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

    SoundActorParam() { Reset(); }

    void Reset() {
        volume = 1.0f;
        pitch = 1.0f;
        tvVolume = 1.0f;
        tvPan = 0.0f;
        lpf = 0.0f;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        biquadFilterType = BiquadFilterType_Min;
        biquadFilterValue = 0.0f;
#endif
    }
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundActorParam) == 0x14);
#else
static_assert(sizeof(SoundActorParam) == 0x1c);
#endif

class BasicSound {
    NN_ATK_RTTI_BASE(BasicSound);

public:
    struct AmbientParamUpdateCallback {};
    struct AmbientArgUpdateCallback {};
    struct AmbientArgAllocatorCallback {
        virtual void Impl1();
        virtual void Impl2();
        virtual void Impl3();
        virtual void RemoveSoundImpl(void* arg, BasicSound* sound);
    };

    struct AmbientInfo {
        AmbientParamUpdateCallback* paramUpdateCallback;
        AmbientArgUpdateCallback* argUpdateCallback;
        AmbientArgAllocatorCallback* argAllocatorCallback;
        void* arg;
        u64 argSize;
    };
    static_assert(sizeof(AmbientInfo) == 0x28);

    enum PlayerState {
        PlayerState_Init,
        PlayerState_Play,
        PlayerState_Stop,
    };

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
    
    bool IsStarted() const { return m_StartedFlag; }

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

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetSend(int bus, float send);
    float GetSend(int bus) const;

    void SetVolumeThroughMode(int bus, u8 modeBitFlag);
    u8 GetVolumeThroughMode(int bus);
    bool IsVolumeThroughModeUsed() const;

    int GetSendBusCount();
    int GetSendChannelCount();
#endif

    void SetPanMode(PanMode mode);
    void SetPanCurve(PanCurve curve);

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputAdditionalParamAddr(OutputDevice device, OutputAdditionalParam* addr,
                                      OutputAdditionalParam* addrForPlayer = nullptr);
#endif
    void SetOutputVolume(OutputDevice device, float volume);
    void SetOutputPan(OutputDevice device, float pan);
    void SetOutputSurroundPan(OutputDevice device, float span);
    void SetOutputMainSend(OutputDevice device, float send);
    void SetOutputFxSend(OutputDevice device, AuxBus bus, float send);

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputAdditionalSend(OutputDevice device, int bus, float send);
    void SetOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus, bool isEnabled);
    void SetOutputBusMixVolume(OutputDevice device, int srcChNo, int subMixBus,
                               ChannelMixVolume param);
#endif

    void SetOutputChannelMixParameter(OutputDevice device, u32 srcChNo, MixParameter param);

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputVolumeThroughMode(OutputDevice device, int bus, u8 modeBitFlag);
#endif

    float GetOutputVolume(OutputDevice device) const;
    float GetOutputPan(OutputDevice device) const;
    float GetOutputSurroundPan(OutputDevice device) const;
    float GetOutputMainSend(OutputDevice device) const;
    float GetOutputFxSend(OutputDevice device, AuxBus bus) const;

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    bool IsOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus) const;
    ChannelMixVolume GetOutputBusMixVolume(OutputDevice device, int srcChNo, int subMixBus) const;
    float GetOutputAdditionalSend(OutputDevice device, int bus) const;
    u8 GetOutputVolumeThroughMode(OutputDevice device, int bus) const;
#endif

    MixParameter GetOutputChannelMixParameter(OutputDevice device, u32 srcChNo) const;

    int GetRemainingFadeFrames() const;
    int GetRemainingPauseFadeFrames() const;
    int GetRemainingMuteFadeFrames() const;

    int GetPlayerPriority() const { return m_Priority; }

    void CalculateSoundParamCalculationValues(SoundParamCalculationValues* pOutValue) const;

    void SetId(u32 id);
    u32 GetId() const { return m_Id; }
    u32 GetInstanceId() const;
    u32 GetPlayFrameCount() const { return m_PlayingCounter; }

    void SetSetupTick(const os::Tick& tick);
    os::Tick GetSetupTick() const { return m_SetupTick; }

    int CalcCurrentPlayerPriority() const {
        return fnd::Clamp(m_AmbientParam.GetPriority(), 0, 127);
    }

    SoundPlayer* GetSoundPlayer() { return m_pSoundPlayer; }
    const SoundPlayer* GetSoundPlayer() const { return m_pSoundPlayer; }

    void AttachSoundPlayer(SoundPlayer* player);
    void DetachSoundPlayer(SoundPlayer* player);

    const SoundActor* GetSoundActor() const { return m_pSoundActor; }

    void AttachSoundActor(SoundActor* actor);
    void DetachSoundActor(SoundActor* actor);

    void AttachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);
    void DetachExternalSoundPlayer(ExternalSoundPlayer* extPlayer);

    void AttachPlayerHeap(PlayerHeap* pHeap);
    void DetachPlayerHeap(PlayerHeap* pHeap);

    PlayerHeap* GetPlayerHeap() { return m_pPlayerHeap; }

    void SetAmbientInfo(const AmbientInfo& ambientArgInfo);

    void ClearAmbientArgUpdateCallback();
    void ClearAmbientParamUpdateCallback();
    void ClearAmbientArgAllocatorCallback();

    const SoundParam& GetAmbientParam() const { return m_AmbientParam; }

    static int GetAmbientPriority(const AmbientInfo& ambientInfo, u32 soundId);

    bool IsAttachedGeneralHandle();
    bool IsAttachedTempGeneralHandle();
    virtual bool IsAttachedTempSpecialHandle() = 0;

    void DetachGeneralHandle();
    void DetachTempGeneralHandle();
    virtual void DetachTempSpecialHandle() = 0;

    void SetUserParamBuffer(void* buffer, size_t size) {
        m_pUserParam = buffer;
        m_UserParamSize = size;
    }

    void* GetUserParam() { return m_pUserParam; }

    void SetSoundStopCallback(const SoundStopCallback callback) { m_SoundStopCallback = callback; }

    void SetSoundArchive(const SoundArchive* soundArchive);
    const SoundArchive* GetSoundArchive() const { return m_pSoundArchive; }

    enum PauseState {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing,
    };
    PauseState GetPauseState() const { return static_cast<PauseState>(m_PauseState); }

    enum MuteState {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting,
    };
    MuteState GetMuteState() const { return static_cast<MuteState>(m_MuteState); }

protected:
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() = 0;
    virtual void OnUpdatePlayerPriority() {}
    virtual void UpdateMoveValue();
    virtual void OnUpdateParam() {}

    bool IsPlayerAvailable() const { return m_PlayerAvailableFlag; }

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
    State m_State{State_Constructed};
    u8 m_PlayerState;
    u8 m_PauseState;
    u8 m_MuteState;
    u8 m_Padding[1];
    s32 m_AutoStopCounter;
    u32 m_UpdateCounter;
    u32 m_PlayingCounter;
    u32 m_Id;
    u32 m_InstanceId;
    os::Tick m_SetupTick{0};
    float m_InitVolume;
    float m_Pitch;
    float m_LpfFreq;
    float m_BiquadFilterValue;
    u32 m_OutputLineFlag;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pOutputReceiver;
#endif

    struct CommonParam {
        MoveValue<float, int> volume;
        MixMode mixMode;
        float pan;
        float span;
        float send[DefaultBusCount];

        void Initialize() {
            volume.InitValue(1.0f);
            mixMode = MixMode_Pan;
            pan = 0.0f;
            span = 0.0f;
            for (int i{0}; i < DefaultBusCount; ++i)
                send[i] = 0.0f;
        }

        void Update() { volume.Update(); }

        float GetVolume() const { return volume.GetValue(); }

        void SetVolume(float target, int frame) { volume.SetTarget(target, frame); }

        CommonParam() = default;
    };
    static_assert(sizeof(CommonParam) == 0x2c);

    CommonParam m_CommonParam;

    OutputParam m_OutputParam[OutputDevice_Count];
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputAdditionalParam* m_pOutputAdditionalParam[OutputDevice_Count]{};
#endif
    void* m_pUserParam{};
    size_t m_UserParamSize{0};
    SoundStopCallback m_SoundStopCallback{};

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
