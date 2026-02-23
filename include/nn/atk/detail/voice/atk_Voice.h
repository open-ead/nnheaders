#pragma once

#include <nn/atk/detail/voice/atk_LowLevelVoice.h>

namespace nn::atk::detail {
class Voice {
public:
    Voice();
    ~Voice();

    void Initialize(u32);

    bool IsAvailable() const;

    bool AllocVoice(u32 priority);
    void Free();

    void SetPriority(u32 priority);
    void SetState(VoiceState state);

    void AppendWaveBuffer(WaveBuffer* waveBuffer);

    void UpdateParam();

    position_t GetPlayPosition() const;

    bool SetMonoFilter(bool enable, u16 cutoff);
    void SetBiquadFilter(bool enable, const BiquadFilterCoefficients* coef);    

    void UpdateVoiceStatus();

private:
    u32 m_Priority;
    VoiceState m_State;
    VoiceParam m_VoiceParam;
    SampleFormat m_SampleFormat;
    u32 m_SampleRate;
    AdpcmParam m_AdpcmParam;
    OutputReceiver* m_pOutputReceiver;
    u32 m_VoiceId;
    position_t m_PlayPosition;
    u32 m_VoiceInfoEnableFlag;
    u32 m_CommandTag;
    WaveBuffer* m_WaveBufferListBegin;
    WaveBuffer* m_WaveBufferListEnd;
    LowLevelVoice* m_pLowLevelVoice;
};
static_assert(sizeof(Voice) == 0xe0);
} // namespace nn::atk::voice