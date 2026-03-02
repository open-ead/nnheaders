#pragma once

#include <atomic>

#include <nn/util.h>
#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_VoiceTypes.h>

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk::detail {
class Voice;

class alignas(256) LowLevelVoice {
public:
    LowLevelVoice();
    
    void Initialize();
    void Finalize();

    void FreeAllWaveBuffer();
    
    bool IsAvailable() const;
    void SetAvailable(bool isAvailable);

    bool IsVoiceDroppedFlagOn() const;

    void AppendWaveBuffer(WaveBuffer* waveBuffer);

    void UpdateState(OutputMode outputMode);
    void UpdateStatePlay(bool isRun, OutputMode outputMode);
    void UpdateStateStop(bool isRun);
    void UpdateStatePause(bool isRun, OutputMode outputMode);

    void SetPriority(s32 priority);

    bool AllocVoice();

    void SetState(VoiceState state);

    void UpdateVoiceInfo(VoiceInfo* voiceInfo) const;
    void UpdateVoiceParam(const VoiceParam& voiceParam, OutputMode outputMode);
    void UpdateVolume(const VoiceParam& voiceParam);
    void UpdatePitch(const VoiceParam& voiceParam);
    void UpdateBiquadFilter(const VoiceParam& voiceParam);
    void UpdateLowPassFilter(const VoiceParam& voiceParam);
    void UpdateMixVolume(const OutputMix& outputMix, OutputMode outputMode);
    void UpdateWaveBuffer(bool isRun, OutputMode outputMode);
    void UpdatePlayPosition();
    void UpdateWaveBufferOnPlayState();
    void UpdateWaveBufferOnStopState(OutputMode outputMode);

    bool AppendWaveBufferToVoice(WaveBuffer* waveBuffer);

    void UpdateMixVolumeOnSubMix(const OutputMix& outputMix, OutputMode outputMode);
    void UpdateMixVolumeOnFinalMix(const OutputMix& outputMix, OutputMode outputMode);

    static f32 GetClampedVoiceVolume(f32 volume);

    void SetVoiceMixVolume(f32 volume, s32 dstIndex);

private:
    AdpcmParam m_AdpcmParam;
    audio::VoiceType m_Voice;
    bool m_IsAvailable;
    bool m_IsSetVoiceSlot;
    VoiceParam m_VoiceParam;
    s32 m_Priority;
    VoiceState m_State;
    u32 m_SampleRate;
    SampleFormat m_SampleFormat;
    position_t m_PlayPosition;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pOutputReceiver;
#endif
    WaveBuffer* m_WaveBufferListBegin;
    WaveBuffer* m_WaveBufferListEnd;
    WaveBuffer* m_LastAppendBuffer;
    Voice* m_pVoice;
    audio::NodeId nodeId;
};
static_assert(sizeof(LowLevelVoice) == 0x100);

class LowLevelVoiceAllocator {
public:
    constexpr static u32 Unassigned = -1;

    LowLevelVoiceAllocator();

    std::size_t GetRequiredMemSize(s32 voiceCount);

    void Initialize(s32 voiceCount, void* mem, std::size_t memSize);
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
} // namespace nn::atk::detail