#include <nn/atk/atk_LowLevelVoice.h>

#include <nn/audio.h>

#include <nn/atk/atk_HardwareManager.h>

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

void LowLevelVoice::Finalize() {
    FreeAllWaveBuffer();

    if (m_IsSetVoiceSlot) {
        audio::ReleaseVoiceSlot(&driver::HardwareManager::GetInstance().GetAudioRendererConfig(),
                                &m_Voice);
        m_IsSetVoiceSlot = false;
    }

    m_pVoice = nullptr;
    m_IsAvailable = false;
    m_State = VoiceState_Stop;
}

bool LowLevelVoice::IsAvailable() const {
    return m_IsAvailable;
}

void LowLevelVoice::SetAvailable(bool isAvailable) {
    m_IsAvailable = isAvailable;
}

bool LowLevelVoice::IsVoiceDroppedFlagOn() const {
    if (audio::IsVoiceValid(&m_Voice))
        return audio::IsVoiceDroppedFlagOn(&m_Voice);

    return false;
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

void LowLevelVoice::FreeAllWaveBuffer() {
    for (WaveBuffer* waveBuffer{m_WaveBufferListBegin}; waveBuffer != nullptr;
         waveBuffer = waveBuffer->next)
        waveBuffer->status = WaveBuffer::Status_Done;

    m_WaveBufferListBegin = nullptr;
    m_WaveBufferListEnd = nullptr;
    m_LastAppendBuffer = nullptr;
}

void LowLevelVoice::UpdateStateStop(bool isRun) {
    if (isRun)
        audio::SetVoicePlayState(&m_Voice, audio::VoiceType::PlayState_Stop);
}

}  // namespace nn::atk::detail