#pragma once

#include <atomic>

#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_VoiceTypes.h>
#include <nn/util.h>

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_OutputReceiver.h>

namespace nn::atk::detail {

class Voice;

class alignas(64) LowLevelVoice {
public:
    LowLevelVoice();

    void Initialize();
    void Finalize();

    bool IsAvailable() const;
    void SetAvailable(bool isAvailable);

    bool IsVoiceDroppedFlagOn() const;

    void AppendWaveBuffer(WaveBuffer* waveBuffer);
    void FreeAllWaveBuffer();

    void UpdateState(OutputMode outputMode);

    void SetVoiceParam(const VoiceParam& voiceParam) { m_VoiceParam = voiceParam; }

    void SetPriority(s32 priority);
    void SetState(VoiceState state);

    void SetSampleRate(s32 sampleRate) { m_SampleRate = sampleRate; }
    void SetSampleFormat(SampleFormat sampleFormat) { m_SampleFormat = sampleFormat; }
    void SetAdpcmParam(const AdpcmParam& adpcmParam) { m_AdpcmParam = adpcmParam; }

    position_t GetPlayPosition() const { return m_PlayPosition; }

    void SetVoice(Voice* pVoice) { m_pVoice = pVoice; }
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetOutputReceiver(OutputReceiver* pOutputReceiver) { m_pOutputReceiver = pOutputReceiver; }
#endif

    void UpdateVoiceInfo(VoiceInfo* voiceInfo) const;

    audio::NodeId GetNodeId() { return m_NodeId; }

private:
    void UpdateVoiceParam(const VoiceParam& voiceParam, OutputMode outputMode);
    bool AllocVoice();

    void UpdateStatePlay(bool isRun, OutputMode outputMode);
    void UpdateStateStop(bool isRun);
    void UpdateStatePause(bool isRun, OutputMode outputMode);
    void UpdatePlayPosition();

    void UpdateWaveBuffer(bool isRun, OutputMode outputMode);
    void UpdateWaveBufferOnPlayState();
    void UpdateWaveBufferOnStopState(OutputMode outputMode);
    bool AppendWaveBufferToVoice(WaveBuffer* waveBuffer);

    void UpdateMixVolume(const OutputMix& outputMix, OutputMode outputMode);
    void UpdateMixVolumeOnSubMix(const OutputMix& outputMix, OutputMode outputMode);
    void UpdateMixVolumeOnFinalMix(const OutputMix& outputMix, OutputMode outputMode);

    void UpdateVolume(const VoiceParam& voiceParam);
    void UpdatePitch(const VoiceParam& voiceParam);
    void UpdateBiquadFilter(const VoiceParam& voiceParam);
    void UpdateLowPassFilter(const VoiceParam& voiceParam);

    void SetVoiceMixVolume(float mixVolume, int destinationIndex);

    float GetClampedVoiceVolume(float volume);

    AdpcmParam m_AdpcmParam{};
    audio::VoiceType m_Voice{};
    bool m_IsAvailable{false};
    bool m_IsSetVoiceSlot{false};
    VoiceParam m_VoiceParam{};
    int32_t m_Priority{0};
    VoiceState m_State{VoiceState_Stop};
    u32 m_SampleRate{32000};
    SampleFormat m_SampleFormat{SampleFormat_PcmS16};
    position_t m_PlayPosition{0};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    u32 _c0{0};  // 0xc0
#else
    OutputReceiver* m_pOutputReceiver{};
#endif
    WaveBuffer* m_WaveBufferListBegin{};
    WaveBuffer* m_WaveBufferListEnd{};
    WaveBuffer* m_LastAppendBuffer{};
    Voice* m_pVoice{};
    audio::NodeId m_NodeId{0};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool _ec{true}; //0xec
#endif
};
static_assert(sizeof(LowLevelVoice) == 0x100);

class LowLevelVoiceAllocator {
public:
    constexpr static u32 Unassigned = -1;

    LowLevelVoiceAllocator();

    size_t GetRequiredMemSize(s32 voiceCount);

    void Initialize(s32 voiceCount, void* mem, size_t memSize);
    void Finalize();

    void UpdateAllVoiceState(OutputMode outputMode);

    LowLevelVoice* AllocVoice();
    u64 GetVoiceArrayIndex(LowLevelVoice* pVoice);
    void FreeVoice(LowLevelVoice* pVoice);

    s32* GetDroppedVoiceCount() const;

private:
    void* m_pVoiceArray{};
    LowLevelVoice** m_ppVoiceTable{};
    s32 m_UsingCount{0};
    s32* m_pAssignedTableIndex{};
    s32 m_VoiceCount{};
    std::atomic<s32> m_DroppedVoiceCount{0};
};
static_assert(sizeof(LowLevelVoiceAllocator) == 0x28);

}  // namespace nn::atk::detail