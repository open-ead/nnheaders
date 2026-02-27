#pragma once

#include <nn/atk/detail/voice/atk_MultiVoice.h>

namespace nn::atk::detail::driver {
class StreamChannel {
public:
    void AppendWaveBuffer(WaveBuffer* pBuffer, bool lastFlag);

private:
    void* m_pBufferAddress;
    MultiVoice* m_pVoice;
    WaveBuffer m_WaveBuffer[32];
    AdpcmContext m_AdpcmContext[32];
    UpdateType m_UpdateType;
};
static_assert(sizeof(StreamChannel) == 0x1080);
} // namespace nn::atk::detail::driver