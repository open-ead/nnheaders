#include <nn/atk/atk_StreamTrack.h>

namespace nn::atk::detail::driver {
void StreamChannel::AppendWaveBuffer(WaveBuffer* pBuffer, bool lastFlag) {
    if (m_pVoice != nullptr)
        m_pVoice->AppendWaveBuffer(0, pBuffer, lastFlag);
}
} // namespace nn::atk::detail::driver