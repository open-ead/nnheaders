#pragma once

#include <nn/atk/detail/atk_OutputAdditionalParam.h>
#include <nn/atk/detail/atk_DisposeCallback.h>
#include <nn/atk/detail/atk_MoveValue.h>
#include <nn/atk/detail/util/atk_CurveAdshr.h>
#include <nn/atk/detail/util/atk_CurveLfo.h>
#include <nn/atk/detail/voice/atk_MultiVoice.h>

namespace nn::atk::detail::driver {
class ChannelManager;

class Channel {
public:
    enum ChannelCallbackStatus {
        ChannelCallbackStatus_Stopped,
        ChannelCallbackStatus_Drop,
        ChannelCallbackStatus_Finish,
        ChannelCallbackStatus_Cancel,
    };

    enum LfoTarget {
        LfoTarget_Pitch,
        LfoTarget_Volume,
        LfoTarget_Pan,
        LfoTarget_Invalid = 0xff,
    };

    using ChannelCallback = void(*)(Channel*,ChannelCallbackStatus,void*);

    constexpr static u32 ChannelCount = 97; 
    constexpr static u32 ChannelMin = 0; 
    constexpr static u32 ChannelMax = 96;

    constexpr static u32 PriorityRelease = 1;
    
    constexpr static u32 KeyInit = 60;
    constexpr static u32 OriginalKeyInit = KeyInit;
    
    constexpr static u8 SilenceVolumeMax = 255;
    constexpr static u8 SilenceVolumeMin = 0;

    constexpr static u32 WaveBufferMax = 2;

    constexpr static u32 ModCount = 4;

    constexpr static f32 SilenceVolumeMaxR = 1.0 / SilenceVolumeMax;

    class Disposer : DisposeCallback {
    public:
        ~Disposer() override;

        void InvalidateData(const void* start, const void* end) override;

    private:
        Channel* m_pChannel;
    };

    Channel();
    ~Channel();

    void InitParam(ChannelCallback callback, void* callbackData);

    void Update(bool doPeriodicProc);
    void Stop();

    s32 GetSweepValue() const;
    void UpdateSweep(s32 count);

    void Start(const WaveInfo& waveInfo, s32 length, position_t startOffsetSamples);
    void Start(const WaveInfo& waveInfo, s32 length, position_t startOffsetSamples, 
               bool isContextCalculationSkipMode);

    void AppendWaveBuffer(const WaveInfo& waveInfo, position_t startOffsetSamples);
    void AppendWaveBuffer(const WaveInfo& waveInfo, position_t startOffsetSamples, 
                          bool isContextCalculationSkipMode);
    
    void Release();
    
    void NoteOff();

    void SetSweepParam(f32 sweepPitch, s32 sweepTime, bool autoUpdate);
    void SetBiquadFilter(s32 type, f32 value);
    void SetTvAdditionalParam(OutputAdditionalParam* param);

    position_t GetCurrentPlayingSample(bool) const;

    static void VoiceCallbackFunc(MultiVoice* voice, 
                                  MultiVoice::VoiceCallbackStatus status, void* arg);

    void CallChannelCallback(ChannelCallbackStatus status);

    static Channel* AllocChannel(s32 voiceChannelCount, s32 priority, 
                          ChannelCallback callback, void* callbackData);

    void FreeChannel();
    void DetachChannel();

private:
    friend ChannelManager;

    Disposer m_Disposer;
    CurveAdshr m_CurveAdshr;
    CurveLfo m_Lfo[4];
    u8 m_LfoTarget[4];
    u8 m_PauseFlag;
    u8 m_ActiveFlag;
    u8 m_AllocFlag;
    u8 m_AutoSweep;
    u8 m_ReleasePriorityFixFlag;
    u8 m_IsIgnoreNoteOff;
    u8 m_BiquadType;
    u8 m_Padding[1];
    f32 m_UserVolume;
    f32 m_UserPitchRatio;
    f32 m_UserLpfFreq;
    f32 m_BiquadValue;
    u32 m_OutputLineFlag;
    detail::OutputParam m_TvParam;
    detail::OutputAdditionalParam* m_pTvAdditionalParam;
    f32 m_UserPitch;
    f32 m_SweepPitch;
    s32 m_SweepCounter;
    s32 m_SweepLength;
    f32 m_InitPan;
    f32 m_InitSurroundPan;
    f32 m_Tune;
    detail::MoveValue<u8, u16> m_SilenceVolume;
    f32 m_Cent;
    f32 m_CentPitch;
    s32 m_Length;
    PanMode m_PanMode;
    PanCurve m_PanCurve;
    u8 m_Key;
    u8 m_OriginalKey;
    u8 m_KeyGroupId;
    u8 m_InterpolationType;
    f32 m_InstrumentVolume;
    f32 m_Velocity;
    ChannelCallback m_Callback;
    void* m_CallbackData;
    MultiVoice* m_pVoice;
    Channel* m_pNextLink;
    WaveBuffer m_WaveBuffer[2][2];
    AdpcmContext m_AdpcmContext[2];
    AdpcmContext m_AdpcmLoopContext[2];
    position_t m_LoopStartFrame;
    position_t m_OriginalLoopStartFrame;
    bool m_LoopFlag;
    util::IntrusiveListNode m_Link;
};
static_assert(sizeof(Channel) == 0x400);
} // namespace nn::atk::detail::driver