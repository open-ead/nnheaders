#include <nn/atk/atk_LowLevelVoice.h>

#include <nn/audio.h>

#include <nn/atk/atk_HardwareManager.h>

namespace {

const s32 LowLevelVoiceInitialPriority{0};
const int LowLevelVoiceInitialSampleRate{32000};
const nn::atk::SampleFormat LowLevelVoiceInitialSampleFormat{nn::atk::SampleFormat_PcmS16};
const nn::atk::detail::VoiceState LowLevelVoiceInitialVoiceState{nn::atk::detail::VoiceState_Stop};

const size_t LowLevelVoiceAlignedSize{sizeof(nn::atk::detail::LowLevelVoice)};

int ConvertAtkPriorityToAudioPriority(s32 atkPriority) {
    if (atkPriority == 255)
        return 0;

    return 255 - atkPriority;
}

int GetOutputReceiverMixBufferIndex(const nn::atk::OutputReceiver* pOutputReceiver, int channel,
                                    int bus) {}

}  // anonymous namespace

namespace nn::atk::detail {

LowLevelVoice::LowLevelVoice()
    : m_Priority{LowLevelVoiceInitialPriority}, m_State{LowLevelVoiceInitialVoiceState},
      m_SampleRate{LowLevelVoiceInitialSampleRate},
      m_SampleFormat{LowLevelVoiceInitialSampleFormat} {}

void LowLevelVoice::Initialize() {
    m_Priority = LowLevelVoiceInitialPriority;
    m_IsAvailable = true;
    m_SampleRate = LowLevelVoiceInitialSampleRate;
    m_SampleFormat = LowLevelVoiceInitialSampleFormat;
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

void LowLevelVoice::UpdateState(OutputMode outputMode) {
    if (!audio::IsVoiceValid(&m_Voice))
        return;

    bool isRun = audio::GetVoicePlayState(&m_Voice) == audio::VoiceType::PlayState_Play;

    switch (m_State) {
    case VoiceState_Play:
        UpdateStatePlay(isRun, outputMode);
        break;

    case VoiceState_Stop:
        UpdateStateStop(isRun);
        break;

    case VoiceState_Pause:
        UpdateStatePause(isRun, outputMode);
        break;
    }
}

void LowLevelVoice::SetPriority(s32 priority) {
    m_Priority = priority;

    if (!audio::IsVoiceValid(&m_Voice))
        return;

    audio::SetVoicePriority(&m_Voice, ConvertAtkPriorityToAudioPriority(m_Priority));
}

void LowLevelVoice::SetState(VoiceState state) {
    m_State = state;

    switch (state) {
    case VoiceState_Play:
        if (!m_IsSetVoiceSlot) {
            AllocVoice();
            m_IsSetVoiceSlot = true;
        }
        break;

    case VoiceState_Stop:
        if (m_IsSetVoiceSlot) {
            audio::ReleaseVoiceSlot(
                &driver::HardwareManager::GetInstance().GetAudioRendererConfig(), &m_Voice);
            m_IsSetVoiceSlot = false;
        }
        break;

    case VoiceState_Pause:
        break;
    }
}

void LowLevelVoice::UpdateStatePlay(bool isRun, OutputMode outputMode) {
    UpdateWaveBuffer(isRun, outputMode);
    UpdateVoiceParam(m_VoiceParam, outputMode);
}

void LowLevelVoice::UpdateStateStop(bool isRun) {
    if (!isRun)
        return;

    audio::SetVoicePlayState(&m_Voice, audio::VoiceType::PlayState_Stop);
}

void LowLevelVoice::UpdateStatePause(bool isRun, OutputMode outputMode) {
    if (!isRun)
        return;

    UpdateVoiceParam(m_VoiceParam, outputMode);
    audio::SetVoicePlayState(&m_Voice, audio::VoiceType::PlayState_Pause);
}

void LowLevelVoice::UpdatePlayPosition() {
    if (m_IsSetVoiceSlot) {
        s64 playedSampleCount{audio::GetVoicePlayedSampleCount(&m_Voice)};
        s64 startOffset{0};

        WaveBuffer* currentWaveBuffer{m_WaveBufferListBegin};

        if (currentWaveBuffer != nullptr) {
            startOffset = currentWaveBuffer->sampleOffset;
            s64 loopSampleLength{currentWaveBuffer->sampleLength - startOffset};

            if (currentWaveBuffer->loopFlag && playedSampleCount > loopSampleLength)
                playedSampleCount =
                    playedSampleCount - (playedSampleCount / loopSampleLength) * loopSampleLength;
        }

        m_PlayPosition = playedSampleCount + startOffset;
    }
}

void LowLevelVoice::UpdateWaveBuffer(bool isRun, OutputMode outputMode) {
    if (isRun) {
        UpdateWaveBufferOnPlayState();
        UpdatePlayPosition();
    } else {
        UpdateWaveBufferOnStopState(outputMode);
    }
}

}  // namespace nn::atk::detail