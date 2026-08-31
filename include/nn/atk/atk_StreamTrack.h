#pragma once

#include <nn/os.h>

#include <nn/atk/atk_MultiVoice.h>
#include <nn/atk/atk_ProfileReader.h>

namespace nn::atk::detail::driver {
struct StreamChannel {
    void AppendWaveBuffer(WaveBuffer* pBuffer, bool lastFlag);
    os::Tick GetProcessTick(const SoundProfile& profile);

    void* m_pBufferAddress;
    MultiVoice* m_pVoice;
    WaveBuffer m_WaveBuffer[32];
    AdpcmContext m_AdpcmContext[32];
    UpdateType m_UpdateType;
};
static_assert(sizeof(StreamChannel) == 0x1080);

class StreamTrack {
public:
    StreamTrack() = default;
    ~StreamTrack();
    
private:
    bool m_ActiveFlag;
    StreamChannel* m_pChannels[2];
    u8 channelCount;
    u8 volume;
    u8 pan;
    u8 span;
    u8 mainSend;
    u8 fxSend[3];
    u8 lpfFreq;
    s8 biquadType;
    u8 biquadValue;
    u8 flags;
    f32 m_Volume;
    s32 m_OutputLine;
    OutputParam m_TvParam;
};
static_assert(sizeof(StreamTrack) == 0x80);
} // namespace nn::atk::detail::driver