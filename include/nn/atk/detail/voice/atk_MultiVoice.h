#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/atk_OutputAdditionalParam.h>
#include <nn/atk/detail/voice/atk_Voice.h>

namespace nn::atk::detail::driver {
class MultiVoice {
public:
    struct PreMixVolume {
        f32 volume[24];
    };
    static_assert(sizeof(PreMixVolume) == 0x60);

    enum VoiceCallbackStatus {
        VoiceCallbackStatus_FinishWave,
        VoiceCallbackStatus_Cancel,
        VoiceCallbackStatus_DropVoice,
        VoiceCallbackStatus_DropDsp,
    };

    using VoiceCallback = void(*)(MultiVoice*,VoiceCallbackStatus,void*);

    constexpr static u32 UpdateStart = 1;
    constexpr static u32 UpdatePause = 2;
    constexpr static u32 UpdateSrc = 4;
    constexpr static u32 UpdateMix = 8;
    constexpr static u32 UpdateLpf = 16;
    constexpr static u32 UpdateBiquad = 32;
    constexpr static u32 UpdateVe = 64;

    constexpr static u32 PriorityNoDrop = 255;

    constexpr static f32 VolumeMin = 0.0;
    constexpr static f32 VolumeDefault = 1.0;
    constexpr static f32 VolumeMax = 2.0;

    constexpr static f32 PanLeft = -1.0;
    constexpr static f32 PanCenter = 0.0;
    constexpr static f32 PanRight = 1.0;

    constexpr static f32 SpanFront = 0.0;
    constexpr static f32 SpanCenter = 1.0;
    constexpr static f32 SpanRear = 2.0;

    constexpr static f32 CutoffFreqMin = 0.0;
    constexpr static f32 CutoffFreqMax = 1.0;

    constexpr static f32 BiquadValueMin = 0.0;
    constexpr static f32 BiquadValueMax = 1.0;

    constexpr static f32 SendMin = 0.0;
    constexpr static f32 SendMax = 1.0;

    explicit MultiVoice();
    explicit MultiVoice(OutputAdditionalParam* pAdditionalParam);
    ~MultiVoice();

    bool Alloc(s32 channelCount, s32 priority, VoiceCallback callback, void* callbackData);
    void InitParam(VoiceCallback callback, void* callbackData);
    void Free();

    void Start();
    void Stop();
    void StopAllSdkVoice();

    void UpdateVoiceStatus();

    bool IsPlayFinished() const;

    void Pause(bool flag);

    void Calc();
    void CalcSrc(bool);
    void CalcVe();
    void CalcMix();
    void CalcLpf();
    void CalcBiquadFilter();

    void Update();

    void RunAllSdkVoice();
    void PauseAllSdkVoice();

    void SetSampleFormat(SampleFormat format);
    void SetSampleRate(s32 sampleRate);
    void SetVolume(f32 volume);
    void SetPitch(f32 pitch);
    void SetPanMode(PanMode panMode);
    void SetPanCurve(PanCurve panCurve);
    void SetLpfFreq(f32 lpfFreq);
    void SetBiquadFilter(s32 type, f32 value);
    void SetPriority(s32 priority);
    void SetOutputLine(u32 lineFlag);
    void SetOutputParamImpl(const OutputParam& in, const OutputParam& out);
    void SetOutputAdditionalParamImpl(SendArray* pAdditionalSend, 
                                      BusMixVolumePacket* pBusMixVolumePacket, 
                                      OutputBusMixVolume* pBusMixVolume, 
                                      VolumeThroughModePacket* pVolumeThroughModePacket);
    void SetOutputBusMixVolumeImpl(BusMixVolumePacket* in, 
                                   OutputBusMixVolume* busMixVolume,
                                   BusMixVolumePacket* out);
    void SetTvParam(const OutputParam& param);
    void SetTvAdditionalParam(OutputAdditionalParam* param);
    void SetTvAdditionalParam(SendArray* pAdditionalSend, 
                              BusMixVolumePacket* pBusMixVolumePacket,
                              OutputBusMixVolume* pBusMixVolume, 
                              VolumeThroughModePacket* pVolumeThroughModePacket);
    void SetOutputReceiver(OutputReceiver* pOutputReceiver);
    void SetSubMixIndex(s32 subMixIndex);

    void CalcPreMixVolume(PreMixVolume* mix, const OutputParam& param, 
                          s32 channelIndex, OutputDevice device);
    void CalcPreMixVolume(PreMixVolume* mix, const OutputParam& param, 
                          OutputAdditionalParam* pAdditionalParam, s32 channelIndex,
                          OutputDevice device);
    void CalcTvMix(OutputMix* mix, const PreMixVolume& pre);
    void CalcMixImpl(OutputMix* mix, u32 outputDeviceIndex, 
                     const OutputParam& param, const PreMixVolume& pre);
    void CalcMixImpl(OutputMix* mix, u32 outputDeviceIndex, const OutputParam& param,
                     OutputAdditionalParam* pAdditionalParam, const PreMixVolume& pre);

    MultiVoice* detail_GetSdkVoice(s32) const;

    position_t GetCurrentPlayingSample() const;
    SampleFormat GetFormat() const;

    bool IsRun() const;

    void SetInterpolationType(u8 interpolationType);

    void AppendWaveBuffer(s32 channelIndex, WaveBuffer* pBuffer, bool lastFlag);

    void SetAdpcmParam(s32 channelIndex, const AdpcmParam& param);

    static u64 FrameToByte(s64, SampleFormat);
    static void CalcOffsetAdpcmParam(AdpcmContext* context, const AdpcmParam& param, 
                                     position_t offsetSamples, const void* dataAddress);
    
private:
    Voice m_Voice[2];
    s32 m_ChannelCount;
    VoiceCallback m_Callback;
    void* m_pCallbackData;
    bool m_IsActive;
    bool m_IsStart;
    bool m_IsStarted;
    bool m_IsPause;
    bool m_IsPausing;
    bool m_IsInitialized;
    WaveBuffer* m_pLastWaveBuffer;
    u16 m_SyncFlag;
    u8 m_BiquadType;
    bool m_IsEnableFrontBypass;
    f32 m_Volume;
    f32 m_Pitch;
    PanMode m_PanMode;
    PanCurve m_PanCurve;
    f32 m_LpfFreq;
    f32 m_BiquadValue;
    s32 m_Priority;
    u32 m_OutputLineFlag;
    OutputParam m_TvParam;
    OutputAdditionalParam* m_pTvAdditionalParam;
    SampleFormat m_Format;
    std::uintptr_t m_VoiceUser;
    UpdateType m_UpdateType;
    OutputReceiver* m_pOutputReceiver;
    util::IntrusiveListNode m_LinkNode;
};
static_assert(sizeof(MultiVoice) == 0x298);
} // namespace nn::atk::detail