#pragma once

#include <nn/atk/atk_CurveAdshr.h>
#include <nn/atk/atk_CurveLfo.h>
#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_MultiVoice.h>
#include <nn/atk/atk_OutputAdditionalParam.h>
#include <nn/atk/atk_ProfileReader.h>

namespace nn::atk::detail::driver {

class ChannelManager;

class Channel {
public:
    enum LfoTarget {
        LfoTarget_Pitch = 0,
        LfoTarget_Volume = 1,
        LfoTarget_Pan = 2,
        LfoTarget_Invalid = 255,
    };

    enum ChannelCallbackStatus {
        ChannelCallbackStatus_Stopped,
        ChannelCallbackStatus_Drop,
        ChannelCallbackStatus_Finish,
        ChannelCallbackStatus_Cancel,
    };

    using ChannelCallback = void (*)(Channel*, ChannelCallbackStatus, void*);

    static const int ChannelCount{193};
    static const int ChannelMin{0};
    static const int ChannelMax{ChannelCount - 1};

    static const int PriorityRelease{1};

private:
    static const int KeyInit{60};
    static const int OriginalKeyInit{KeyInit};

    static const u8 SilenceVolumeMax{255};
    static const u8 SilenceVolumeMin{0};
    static const float SilenceVolumeMaxR;  // = 1.0 / SilenceVolumeMax;

public:
    static Channel* AllocChannel(int voiceChannelCount, int priority, ChannelCallback callback,
                                 void* callbackData);

    static void FreeChannel(Channel* channel);
    static void DetachChannel(Channel* channel);

private:
    static void VoiceCallbackFunc(MultiVoice* voice, MultiVoice::VoiceCallbackStatus status,
                                  void* arg);

public:
    static const int WaveBufferMax{2};

    Channel();
    ~Channel();

    void Update(bool doPeriodicProc);

    void CallChannelCallback(ChannelCallbackStatus status);

    void Start(const WaveInfo& waveInfo, int length, position_t startOffsetSamples);
    void Start(const WaveInfo& waveInfo, int length, position_t startOffsetSamples,
               bool isContextCalculationSkipMode);

    void Stop();
    void Pause(bool flag);

    void NoteOff();
    void Release();

    bool IsActive() const { return m_ActiveFlag != 0; }
    bool IsPause() const { return m_PauseFlag != 0; }

    void SetKey(u8 key) { m_Key = key; }

    void SetKey(u8 key, u8 originalKey) {
        m_Key = key;
        m_OriginalKey = originalKey;
    }

    void SetInitPan(float pan) { m_InitPan = pan; }
    void SetInitSurroundPan(float span) { m_InitSurroundPan = span; }
    void SetTune(float tune) { m_Tune = tune; }

    void SetAttack(int attack) { m_CurveAdshr.SetAttack(attack); }
    void SetHold(int hold) { m_CurveAdshr.SetHold(hold); }
    void SetDecay(int decay) { m_CurveAdshr.SetDecay(decay); }
    void SetSustain(int sustain) { m_CurveAdshr.SetSustain(sustain); }
    void SetRelease(int release) { m_CurveAdshr.SetRelease(release); }

    void SetSilence(bool silenceFlag, int fadeTimes) {
        m_SilenceVolume.SetTarget(silenceFlag ? SilenceVolumeMin : SilenceVolumeMax, fadeTimes);
    }

    int GetLength() const { return m_Length; }
    void SetLength(s32 length) { m_Length = length; }

    bool IsRelease() const { return m_CurveAdshr.GetStatus() == CurveAdshr::Status_Release; }

    void SetUserVolume(float volume) { m_UserVolume = volume; }
    void SetUserPitch(float pitch) { m_UserPitch = pitch; }
    void SetUserPitchRatio(float pitchRatio) { m_UserPitchRatio = pitchRatio; }
    void SetUserLpfFreq(float lpfFreq) { m_UserLpfFreq = lpfFreq; }

    void SetBiquadFilter(int type, float value);

    void SetLfoParam(const CurveLfoParam& param, int i) { m_Lfo[i].SetParam(param); }
    void SetLfoTarget(LfoTarget type, int i) { m_LfoTarget[i] = type; }

    void SetPriority(int priority);
    
    void SetReleasePriorityFix(bool fix) { m_ReleasePriorityFixFlag = fix; }
    void SetIsIgnoreNoteOff(bool flag) { m_IsIgnoreNoteOff = flag; }

    void SetSweepParam(float sweepPitch, int sweepTime, bool autoUpdate) {
        m_SweepPitch = sweepPitch;
        m_SweepLength = sweepTime;
        m_AutoSweep = autoUpdate;
        m_SweepCounter = 0;
    }

    bool IsAutoUpdateSweep() const { return m_AutoSweep != 0; }
    void UpdateSweep(int count);

    void SetPanMode(PanMode panMode) { m_PanMode = panMode; }
    void SetPanCurve(PanCurve panCurve) { m_PanCurve = panCurve; }

    void SetOutputLine(u32 lineFlag) { m_OutputLineFlag = lineFlag; }
    u32 GetOutputLine() const { return m_OutputLineFlag; }

    void SetTvParam(const OutputParam& param) { m_TvParam = param; }
    const OutputParam& GetTvParam() const { return m_TvParam; }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetTvAdditionalParamAddr(OutputAdditionalParam* param) { m_pTvAdditionalParam = param; }
    const OutputAdditionalParam* const GetTvAdditionalParamAddr() const {
        return m_pTvAdditionalParam;
    }
    OutputAdditionalParam* GetTvAdditionalParamAddr() { return m_pTvAdditionalParam; }
    void SetTvAdditionalParam(const OutputAdditionalParam& param);
#endif
    void SetMainSend(float mainSend);
    void SetUserPan(float pan);

    Channel* GetNextTrackChannel() const { return m_pNextLink; }
    void SetNextTrackChannel(Channel* channel) { m_pNextLink = channel; }

    position_t GetCurrentPlayingSample(bool isOriginalSamplePosition) const;

    void SetKeyGroupId(u8 id) { m_KeyGroupId = id; }
    u8 GetKeyGroupId() const { return m_KeyGroupId; }

    void SetInterpolationType(u8 type) { m_InterpolationType = type; }
    u8 GetInterpolationType() const { return m_InterpolationType; }

    void SetInstrumentVolume(float instrumentVolume) { m_InstrumentVolume = instrumentVolume; }
    void SetVelocity(float velocity) { m_Velocity = velocity; }

    static const int ModCount{4};

    void SetUpdateType(UpdateType updateType) { m_pVoice->SetUpdateType(updateType); }
    UpdateType GetUpdateType() const { return m_pVoice->GetUpdateType(); }

    void SetOutputReceiver(OutputReceiver* pOutputReceiver) {
        m_pVoice->SetOutputReceiver(pOutputReceiver);
    }

    os::Tick GetProcessTick(const SoundProfile& profile) {
        return m_pVoice->GetProcessTick(profile);
    }

private:
    class Disposer : public DisposeCallback {
    public:
        Disposer() = default;

        void Initialize(Channel* channel) { m_pChannel = channel; }

        ~Disposer() override;

        void InvalidateData(const void* start, const void* end) override;

    private:
        Channel* m_pChannel{};
    };

    int GetSweepValue() const;
    void InitParam(ChannelCallback callback, void* callbackData);

    void AppendWaveBuffer(const WaveInfo& waveInfo, position_t startOffsetSamples);
    void AppendWaveBuffer(const WaveInfo& waveInfo, position_t startOffsetSamples,
                          bool isContextCalculationSkipMode);

    Disposer m_Disposer;
    CurveAdshr m_CurveAdshr;
    CurveLfo m_Lfo[ModCount];
    u8 m_LfoTarget[ModCount];
    u8 m_PauseFlag{0};
    u8 m_ActiveFlag{0};
    u8 m_AllocFlag{0};
    u8 m_AutoSweep;
    u8 m_ReleasePriorityFixFlag;
    u8 m_IsIgnoreNoteOff;
    u8 m_BiquadType;
    u8 m_Padding[1];
    float m_UserVolume;
    float m_UserPitchRatio;
    float m_UserLpfFreq;
    float m_BiquadValue;
    u32 m_OutputLineFlag;
    OutputParam m_TvParam;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    detail::OutputAdditionalParam* m_pTvAdditionalParam{};
#endif
    float m_UserPitch;
    float m_SweepPitch;
    int m_SweepCounter;
    int m_SweepLength;
    float m_InitPan;
    float m_InitSurroundPan;
    float m_Tune;
    MoveValue<u8, u16> m_SilenceVolume;
    float m_Cent;
    float m_CentPitch;
    int m_Length;
    PanMode m_PanMode;
    PanCurve m_PanCurve;
    u8 m_Key;
    u8 m_OriginalKey;
    u8 m_KeyGroupId;
    u8 m_InterpolationType;
    float m_InstrumentVolume;
    float m_Velocity;
    ChannelCallback m_Callback;
    void* m_CallbackData;
    MultiVoice* m_pVoice{};
    Channel* m_pNextLink;
    WaveBuffer m_WaveBuffer[WaveBufferMax][WaveChannelMax];
    AdpcmContext m_AdpcmContext[WaveChannelMax];
    AdpcmContext m_AdpcmLoopContext[WaveChannelMax];
    position_t m_LoopStartFrame;
    position_t m_OriginalLoopStartFrame;
    bool m_LoopFlag;

public:
    util::IntrusiveListNode m_Link;
};
static_assert(sizeof(Channel) == 0x400);

}  // namespace nn::atk::detail::driver