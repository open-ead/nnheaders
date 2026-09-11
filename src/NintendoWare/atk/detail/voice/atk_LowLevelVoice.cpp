#include <nn/atk/atk_LowLevelVoice.h>

namespace nn::atk::detail {

LowLevelVoice::LowLevelVoice() = default;

void LowLevelVoice::Initialize() {
    m_Priority = 0;
    m_IsAvailable = true;
    m_SampleRate = 32000;
    m_SampleFormat = SampleFormat_PcmS16;
    m_PlayPosition = 0;
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
    _c0 = 0;
#else
    m_pOutputReceiver = nullptr;
#endif
    m_NodeId = 0;
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
    _ec = true;
#endif
}

void LowLevelVoice::AppendWaveBuffer(WaveBuffer* waveBuffer) {
    waveBuffer->next = nullptr;
    waveBuffer->status = WaveBuffer::Status_Wait;
    if (m_WaveBufferListEnd == nullptr)
        m_WaveBufferListBegin = waveBuffer;
    else
        m_WaveBufferListEnd->next = waveBuffer;

    m_WaveBufferListEnd = waveBuffer;
}

}  // namespace nn::atk::detail