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

void Channel::Start(const WaveInfo& waveInfo, int length, position_t startOffsetSamples, bool isContextCalculationSkipMode) {
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

}  // namespace nn::atk::detail::driver