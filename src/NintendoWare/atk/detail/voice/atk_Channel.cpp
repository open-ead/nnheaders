#include <nn/atk/atk_Channel.h>

#include <nn/atk/atk_ChannelManager.h>
#include <nn/atk/atk_DisposeCallbackManager.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_MultiVoiceManager.h>

namespace nn::atk::detail::driver {

namespace {

u8 GetNwInterpolationTypeFromHardwareManager() {
    u8 result{0};

    switch (HardwareManager::GetInstance().GetSrcType()) {
    case SampleRateConverterType_None:
        result = 2;
        break;

    case SampleRateConverterType_Linear:
        result = 1;
        break;

    case SampleRateConverterType_4Tap:
        // result = 0;
        break;
    }

    return result;
}

}  // anonymous namespace

Channel* Channel::AllocChannel(int voiceChannelCount, int priority, ChannelCallback callback,
                               void* callbackData) {
    Channel* channel{ChannelManager::GetInstance()->Alloc()};

    if (channel == nullptr) {
        Util::WarningLogger::GetInstance().Log(0, 0, 0);
        return nullptr;
    }

    channel->m_AllocFlag = 1;

    MultiVoice* voice{MultiVoiceManager::GetInstance()->AllocVoice(voiceChannelCount, priority,
                                                                   VoiceCallbackFunc, channel)};

    if (voice == nullptr) {
        FreeChannel(channel);
        return nullptr;
    }

    channel->m_pVoice = voice;
    channel->InitParam(callback, callbackData);

    return channel;
}

void Channel::FreeChannel(Channel* channel) {
    ChannelManager::GetInstance()->Free(channel);
}

void Channel::DetachChannel(Channel* channel) {
    channel->m_Callback = nullptr;
    channel->m_CallbackData = nullptr;
}

void Channel::VoiceCallbackFunc(MultiVoice* voice, MultiVoice::VoiceCallbackStatus status,
                                void* arg) {
    ChannelCallbackStatus chStatus{ChannelCallbackStatus_Finish};
    auto* channel{static_cast<Channel*>(arg)};

    switch (status) {
    case MultiVoice::VoiceCallbackStatus_FinishWave:
        voice->Free();
        chStatus = ChannelCallbackStatus_Finish;
        break;

    case MultiVoice::VoiceCallbackStatus_Cancel:
        voice->Free();
        chStatus = ChannelCallbackStatus_Cancel;
        break;

    case MultiVoice::VoiceCallbackStatus_DropVoice:
        chStatus = ChannelCallbackStatus_Drop;
        break;

    case MultiVoice::VoiceCallbackStatus_DropDsp:
        chStatus = ChannelCallbackStatus_Drop;
        break;
    }

    channel->CallChannelCallback(chStatus);

    channel->m_pVoice = nullptr;
    channel->m_PauseFlag = 0;
    channel->m_ActiveFlag = 0;
    channel->m_AllocFlag = 0;
}

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

void Channel::SetBiquadFilter(int type, float value) {
    m_BiquadType = static_cast<u8>(type);
    m_BiquadValue = value;
}

void Channel::SetSweepParam(float sweepPitch, int sweepTime, bool autoUpdate) {
    m_SweepPitch = sweepPitch;
    m_SweepLength = sweepTime;
    m_AutoSweep = autoUpdate;
    m_SweepCounter = 0;
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

void Channel::Disposer::InvalidateData(const void* start, const void* end) {
    if (m_pChannel->m_pVoice == nullptr)
        return;

    bool disposeFlag{false};
    int sdkVoiceCount{m_pChannel->m_pVoice->GetSdkVoiceCount()};

    for (int channelIndex{0}; channelIndex < sdkVoiceCount; ++channelIndex) {
        for (int waveBufferIndex{0}; waveBufferIndex < WaveBufferMax; ++waveBufferIndex) {
            const WaveBuffer& waveBuffer{m_pChannel->m_WaveBuffer[channelIndex][waveBufferIndex]};

            if (waveBuffer.status == WaveBuffer::Status_Done)
                continue;

            const void* bufferEnd{
                util::ConstBytePtr(waveBuffer.bufferAddress,
                                   Util::GetByteBySample(waveBuffer.sampleLength,
                                                         m_pChannel->m_pVoice->GetFormat()))
                    .Get()};

            if (start <= bufferEnd && end >= waveBuffer.bufferAddress) {
                disposeFlag = true;
                break;
            }
        }
    }

    if (disposeFlag) {
        m_pChannel->CallChannelCallback(ChannelCallbackStatus_Cancel);
        m_pChannel->Stop();
        Channel::FreeChannel(m_pChannel);
    }
}

float Channel::GetSweepValue() const {
    if (m_SweepPitch == 0.0f)
        return 0.0f;

    if (m_SweepCounter >= m_SweepLength)
        return 0.0f;

    float sweep{m_SweepPitch};
    sweep *= static_cast<float>(m_SweepLength - m_SweepCounter);
    sweep /= static_cast<float>(m_SweepLength);

    return sweep;
}

void Channel::InitParam(ChannelCallback callback, void* callbackData) {
    m_pNextLink = nullptr;

    m_Callback = callback;
    m_CallbackData = callbackData;

    m_PauseFlag = 0;
    m_AutoSweep = 1;
    m_ReleasePriorityFixFlag = 0;
    m_IsIgnoreNoteOff = 0;

    m_LoopFlag = false;
    m_LoopStartFrame = 0;
    m_OriginalLoopStartFrame = 0;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    m_StartOffsetSamples = 0;
#endif
    m_Length = 0;

    m_Key = KeyInit;
    m_OriginalKey = OriginalKeyInit;

    m_InitPan = 0.0f;
    m_InitSurroundPan = 0.0f;

    m_Tune = 1.0f;

    m_Cent = 0.0f;
    m_CentPitch = 1.0f;

    m_UserVolume = 1.0f;
    m_UserPitch = 0.0f;
    m_UserPitchRatio = 1.0f;
    m_UserLpfFreq = 0.0f;

    m_BiquadType = BiquadFilterType_None;
    m_BiquadValue = 0.0f;

    m_OutputLineFlag = OutputLine_Main;

    m_TvParam.Initialize();
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    if (m_pTvAdditionalParam != nullptr)
        m_pTvAdditionalParam->Reset();
#endif

    m_SilenceVolume.InitValue(SilenceVolumeMax);

    m_SweepPitch = 0.0f;
    m_SweepCounter = 0;
    m_SweepLength = 0;

    m_CurveAdshr.Initialize();
    for (int i{0}; i < ModCount; ++i) {
        m_Lfo[i].GetParam().Initialize();
        m_LfoTarget[i] = LfoTarget_Invalid;
    }

    m_PanMode = PanMode_Dual;
    m_PanCurve = PanCurve_Sqrt;

    m_KeyGroupId = 0;

    m_InterpolationType = GetNwInterpolationTypeFromHardwareManager();

    m_InstrumentVolume = 1.0f;

    m_Velocity = 1.0f;
}

}  // namespace nn::atk::detail::driver