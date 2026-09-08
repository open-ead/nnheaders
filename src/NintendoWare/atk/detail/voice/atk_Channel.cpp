#include <nn/atk/atk_Channel.h>

#include <nn/atk/atk_DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

namespace {

u8 GetNwInterpolationTypeFromHardwareManager() {
    // TODO
    u8 result;
}

}  // anonymous namespace

Channel::Channel() {
    m_Disposer.Initialize(this);
    DisposeCallbackManager::GetInstance()->RegisterDisposeCallback(&m_Disposer);
}

Channel::~Channel() {
    DisposeCallbackManager::GetInstance()->UnregisterDisposeCallback(&m_Disposer);
}

void Channel::CallChannelCallback(ChannelCallbackStatus status) {
    if (m_Callback == nullptr)
        return;

    m_Callback(this, status, m_CallbackData);
    m_Callback = nullptr;
    m_CallbackData = nullptr;
}

void Channel::Start(const WaveInfo& waveInfo, int length, position_t startOffsetSamples) {
    m_Length = length;

    for (int i{0}; i < ModCount; ++i)
        m_Lfo[i].Reset();

    m_CurveAdshr.Reset();

    m_SweepCounter = 0;
    m_pVoice->SetSampleFormat(waveInfo.sampleFormat);
    m_pVoice->SetSampleRate(waveInfo.sampleRate);
    m_pVoice->SetInterpolationType(m_InterpolationType);

    AppendWaveBuffer(waveInfo, startOffsetSamples);

    m_pVoice->Start();

    m_ActiveFlag = 1;
}

void Channel::Start(const WaveInfo& waveInfo, int length, position_t startOffsetSamples,
                    bool isContextCalculationSkipMode) {
    m_Length = length;

    for (int i{0}; i < ModCount; ++i)
        m_Lfo[i].Reset();

    m_CurveAdshr.Reset();

    m_SweepCounter = 0;
    m_pVoice->SetSampleFormat(waveInfo.sampleFormat);
    m_pVoice->SetSampleRate(waveInfo.sampleRate);
    m_pVoice->SetInterpolationType(m_InterpolationType);

    AppendWaveBuffer(waveInfo, startOffsetSamples, isContextCalculationSkipMode);

    m_pVoice->Start();

    m_ActiveFlag = 1;
}

void Channel::Stop() {
    if (m_pVoice == nullptr)
        return;

    m_pVoice->Stop();
    m_pVoice->Free();

    m_pVoice = nullptr;
    m_PauseFlag = 0;
    m_ActiveFlag = 0;
}

void Channel::NoteOff() {
    if (m_IsIgnoreNoteOff != 0)
        return;

    Release();
}

void Channel::Release() {
    if (!IsRelease()) {
        if (m_pVoice != nullptr && m_ReleasePriorityFixFlag == 0)
            m_pVoice->SetPriority(PriorityRelease);

        m_CurveAdshr.SetStatus(CurveAdshr::Status_Release);
    }

    m_PauseFlag = 0;
}

void Channel::UpdateSweep(int count) {
    m_SweepCounter =
        m_SweepCounter + count <= m_SweepLength ? m_SweepCounter + count : m_SweepLength;
}

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
void Channel::SetTvAdditionalParam(const OutputAdditionalParam& param) {
    if (m_pTvAdditionalParam != nullptr)
        *m_pTvAdditionalParam = param;
}
#endif

position_t Channel::GetCurrentPlayingSample(bool isOriginalSamplePosition) const {
    if (m_ActiveFlag == 0)
        return 0;

    position_t playSamplePosition{m_pVoice->GetCurrentPlayingSample()};

    if (isOriginalSamplePosition && m_LoopFlag) {
        position_t loopEnd{m_WaveBuffer[0][0].sampleLength +
                           (-m_LoopStartFrame + m_OriginalLoopStartFrame)};
        position_t originalLoopEnd{m_OriginalLoopStartFrame + playSamplePosition - loopEnd};

        if (playSamplePosition > loopEnd)
            playSamplePosition = originalLoopEnd;
    }

    return playSamplePosition;
}

}  // namespace nn::atk::detail::driver