#include <nn/atk/atk_BasicSound.h>

#include <nn/atk/atk_DriverCommand.h>
#include <nn/atk/atk_ExternalSoundPlayer.h>
#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/atk_SoundPlayer.h>

namespace nn::atk::detail {

namespace {

void SetSoundPlayerCalculationValues(SoundParamCalculationValues::SoundPlayerParam& param,
                                     const SoundPlayer& player) {
    param.volume = player.GetVolume();
    param.lpf = player.GetLowPassFilterFrequency();
    param.bqfType = player.GetBiquadFilterType();
    param.bqfValue = player.GetBiquadFilterValue();

    for (int i{0}; i < OutputDevice_Count; ++i) {
        const OutputDevice device{static_cast<OutputDevice>(i)};

        param.outputVolume[device] = player.GetOutputVolume(device);
        param.outputMainSend[device] = player.GetOutputMainSend(device);

        for (int k{0}; k < AuxBus_Count; ++k) {
            const AuxBus bus{static_cast<AuxBus>(k)};
            param.outputEffectSend[device][bus] = player.GetOutputFxSend(device, bus);
        }
    }
}

void SetSound3DCalculationValues(SoundParamCalculationValues::Sound3DParam& param,
                                 const SoundParam& ambientParam) {
    param.volume = ambientParam.GetVolume();
    param.pitch = ambientParam.GetPitch();
    param.lpf = ambientParam.GetLpf();
    param.bqfType = ambientParam.GetBiquadFilterType();
    param.bqfValue = ambientParam.GetBiquadFilterValue();
    param.outputLineFlag = ambientParam.GetOutputLineFlag();
    param.playerPriority = ambientParam.GetPriority();

    for (int i{0}; i < OutputDevice_Count; ++i) {
        const OutputAmbientParam* pAmbientParam{&ambientParam.GetTvParam()};
        const OutputDevice device{static_cast<OutputDevice>(i)};

        param.outputVolume[device] = pAmbientParam->GetVolume();
        param.outputPan[device] = pAmbientParam->GetPan();
        param.outputSurroundPan[device] = pAmbientParam->GetSurroundPan();

        for (int k{0}; k < AuxBus_Count; ++k) {
            const AuxBus bus{static_cast<AuxBus>(k)};
            param.outputEffectSend[device][bus] = pAmbientParam->GetEffectSend(bus);
        }
    }
}

void SetSoundActorCalculationValues(SoundParamCalculationValues::SoundActorParam& param,
                                    const SoundActorParam& actorParam) {
    param.volume = actorParam.volume;
    param.pitch = actorParam.pitch;
    param.lpf = actorParam.lpf;

    for (int i{0}; i < OutputDevice_Count; ++i) {
        const OutputDevice device{static_cast<OutputDevice>(i)};

        param.outputVolume[device] = actorParam.tvVolume;
        param.outputPan[device] = actorParam.tvPan;
    }
}

}  // anonymous namespace

// NON_MATCHING on versions lower than 4.0.0
BasicSound::BasicSound() = default;

// NON_MATCHING: bad order of instructions
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
bool BasicSound::Initialize()
#else
bool BasicSound::Initialize(OutputReceiver* pOutputReceiver)
#endif
{
    m_InstanceId = g_LastInstanceId++;
    // ++g_LastInstanceId;

    m_pPlayerHeap = nullptr;
    m_pExtSoundPlayer = nullptr;
    m_pSoundArchive = nullptr;

    m_PlayerState = PlayerState_Init;
    m_PauseState = PauseState_Normal;

    m_pSoundPlayer = nullptr;
    m_pSoundActor = nullptr;
    m_MuteState = MuteState_Normal;

    m_pGeneralHandle = nullptr;
    m_pTempGeneralHandle = nullptr;

    m_PauseMode = PauseMode_Default;

    m_BiquadFilterType = BiquadFilterType_Inherit;

    m_AutoStopCounter = 0;
    m_UpdateCounter = 0;
    m_PlayingCounter = 0;
    m_Id = 0xffffffff;

    m_AmbientInfo.paramUpdateCallback = nullptr;
    m_AmbientInfo.argUpdateCallback = nullptr;
    m_AmbientInfo.argAllocatorCallback = nullptr;
    m_AmbientInfo.arg = nullptr;
    m_AmbientInfo.argSize = 0;

    m_StartFlag = false;
    m_StartedFlag = false;
    m_AutoStopFlag = false;
    m_FadeOutFlag = false;
    m_PlayerAvailableFlag = false;
    m_UnPauseFlag = false;

    m_FadeVolume.InitValue(1.0f);
    m_PauseFadeVolume.InitValue(1.0f);
    m_MuteFadeVolume.InitValue(1.0f);

    m_InitVolume = 1.0f;
    m_Pitch = 1.0f;
    m_LpfFreq = 0.0f;
    m_BiquadFilterValue = 0.0f;
    m_OutputLineFlag = OutputLine_Main;
    // m_Priority = 0;

    m_CommonParam.Initialize();

    for (int i{0}; i < OutputDevice_Count; ++i) {
        m_OutputParam[i].Initialize();
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        if (m_pOutputAdditionalParam[i] != nullptr)
            m_pOutputAdditionalParam[i]->Reset();
#endif
    }

    m_AmbientParam.Initialize();
    m_ActorParam.Reset();

    if (m_UserParamSize != 0)
        std::memset(m_pUserParam, 0, m_UserParamSize);

    {
        driver::BasicSoundPlayer* basicPlayer{GetBasicSoundPlayerHandle()};

        DriverCommand& cmdmgr{*DriverCommand::GetInstance()};

        auto* command{cmdmgr.AllocCommand<DriverCommandPlayerInit>()};
        command->id = DriverCommandId_PlayerInit;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        command->pOutputReceiver = pOutputReceiver;
#endif
        command->availableFlagPtr = &m_PlayerAvailableFlag;
        command->player = basicPlayer;

        cmdmgr.PushCommand(command);

        basicPlayer->InitializeEvent();
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    pOutputReceiver->AddReferenceCount(1);
    m_pOutputReceiver = pOutputReceiver;
#endif
    m_State = State_Initialized;
    return true;
}

void BasicSound::Finalize() {
    if (m_State != State_Initialized)
        return;

    SetId(0xffffffff);

    if (IsAttachedGeneralHandle())
        DetachGeneralHandle();

    if (IsAttachedTempGeneralHandle())
        DetachTempGeneralHandle();

    if (IsAttachedTempSpecialHandle())
        DetachTempSpecialHandle();

    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_RemoveSound(this);

    if (m_pExtSoundPlayer != nullptr)
        m_pExtSoundPlayer->RemoveSound(this);

    if (m_AmbientInfo.argAllocatorCallback != nullptr) {
        m_AmbientInfo.argAllocatorCallback->RemoveSoundImpl(m_AmbientInfo.arg, this);
        m_AmbientInfo.arg = nullptr;
    }

    {
        if (m_StartedFlag) {
            DriverCommand& cmdmgr{*DriverCommand::GetInstance()};

            auto* command{cmdmgr.AllocCommand<DriverCommandPlayer>()};
            command->id = DriverCommandId_PlayerStop;
            command->player = GetBasicSoundPlayerHandle();
            command->flag = m_FadeOutFlag;

            cmdmgr.PushCommand(command);

            m_StartedFlag = false;
        }

        m_PlayerAvailableFlag = false;
        m_PlayerState = PlayerState_Stop;

        DriverCommand& cmdmgr{*DriverCommand::GetInstance()};

        auto* command{cmdmgr.AllocCommand<DriverCommandPlayer>()};
        command->id = DriverCommandId_PlayerFinalize;
        command->player = GetBasicSoundPlayerHandle();

        cmdmgr.PushCommand(command);
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    m_pOutputReceiver = nullptr;
#endif

    if (m_SoundStopCallback != nullptr) {
        m_SoundStopCallback();
        m_SoundStopCallback = nullptr;
    }
    m_FadeOutFlag = false;
    m_State = State_Finalized;
}

void BasicSound::StartPrepared() {
    m_StartFlag = true;
}

void BasicSound::Stop(int fadeFrames) {
    if ((fadeFrames <= 0 || m_PauseState == PauseState_Paused) ||
        (!m_StartFlag && !m_StartedFlag)) {
        Finalize();
        return;
    }

    int frames{static_cast<int>(fadeFrames * m_FadeVolume.GetValue())};
    m_FadeVolume.SetTarget(0.0f, frames);
    SetPlayerPriority(0);

    m_AutoStopFlag = false;
    m_PauseState = PauseState_Normal;
    m_UnPauseFlag = false;
    m_PauseMode = PauseMode_Default;
    m_FadeOutFlag = true;
    m_MuteState = MuteState_Normal;
}

void BasicSound::ForceStop() {
    m_FadeOutFlag = true;

    Finalize();
}

void BasicSound::Pause(bool flag, int fadeFrames) {
    Pause(flag, fadeFrames, PauseMode_Default);
}

void BasicSound::Pause(bool flag, int fadeFrames, PauseMode pauseMode) {
    int frames;

    if (flag) {
        switch (m_PauseState) {
        case PauseState_Normal:
        case PauseState_Unpausing:
        case PauseState_Pausing:
            frames = static_cast<int>(fadeFrames * m_PauseFadeVolume.GetValue());
            frames = frames > 0 ? frames : 1;
            m_PauseFadeVolume.SetTarget(0.0f, frames);
            m_PauseState = PauseState_Pausing;
            m_UnPauseFlag = false;
            break;
        case PauseState_Paused:
        default:
            return;
        }
    } else {
        switch (m_PauseState - 1) {
        case PauseState_Normal:
        case PauseState_Pausing:
        case PauseState_Paused:
            frames = static_cast<int>(fadeFrames * (1.0f - m_PauseFadeVolume.GetValue()));
            frames = frames > 0 ? frames : 1;
            m_PauseFadeVolume.SetTarget(1.0f, frames);
            m_PauseState = PauseState_Unpausing;
            m_UnPauseFlag = true;
            break;
        case PauseState_Unpausing:
        default:
            return;
        }
    }

    m_PauseMode = pauseMode;
}

void BasicSound::Mute(bool flag, int fadeFrames) {
    int frames;

    if (flag) {
        switch (m_MuteState) {
        case MuteState_Normal:
        case MuteState_Muting:
        case MuteState_Unmuting:
            frames = static_cast<int>(fadeFrames * m_MuteFadeVolume.GetValue());
            frames = frames > 0 ? frames : 1;
            m_MuteFadeVolume.SetTarget(0.0f, frames);
            m_MuteState = MuteState_Muting;
            break;
        case MuteState_Muted:
        default:
            return;
        }
    } else {
        switch (m_MuteState - 1) {
        case MuteState_Normal:
        case MuteState_Muting:
        case MuteState_Muted:
            frames = static_cast<int>(fadeFrames * (1.0f - m_MuteFadeVolume.GetValue()));
            frames = frames > 0 ? frames : 1;
            m_MuteFadeVolume.SetTarget(1.0f, frames);
            m_MuteState = MuteState_Unmuting;
            break;
        case MuteState_Unmuting:
        default:
            return;
        }
    }
}

void BasicSound::SetAutoStopCounter(int frames) {
    m_AutoStopCounter = frames;
    m_AutoStopFlag = frames > 0;
}

void BasicSound::FadeIn(int frames) {
    if (m_FadeOutFlag || m_UpdateCounter > 0)
        return;

    m_FadeVolume.InitValue(0.0f);
    m_FadeVolume.SetTarget(1.0f, frames);
}

bool BasicSound::IsPause() const {
    switch (m_PauseState - 1) {
    case PauseState_Normal:
    case PauseState_Pausing:
        return true;
    case PauseState_Paused:
    case PauseState_Unpausing:
    default:
        return false;
    }
}

bool BasicSound::IsMute() const {
    switch (m_MuteState - 1) {
    case MuteState_Normal:
    case MuteState_Muting:
        return true;
    case MuteState_Muted:
    case MuteState_Unmuting:
    default:
        return false;
    }
}

void BasicSound::SetPriority(int priority, int ambientPriority) {
    m_Priority = priority;
    m_AmbientParam.SetPriority(ambientPriority);
}

void BasicSound::GetPriority(int* priority, int* ambientPriority) const {
    if (priority != nullptr)
        *priority = m_Priority;

    if (ambientPriority != nullptr)
        *ambientPriority = m_AmbientParam.GetPriority();
}

void BasicSound::SetInitialVolume(float volume) {
    m_InitVolume = volume < 0.0f ? 0.0f : volume;
}

float BasicSound::GetInitialVolume() const {
    return m_InitVolume;
}

void BasicSound::SetVolume(float volume, int frames) {
    m_CommonParam.SetVolume(volume < 0.0f ? 0.0f : volume, frames);
}

float BasicSound::GetVolume() const {
    return m_CommonParam.GetVolume();
}

void BasicSound::SetPitch(float pitch) {
    m_Pitch = pitch;
}

float BasicSound::GetPitch() const {
    return m_Pitch;
}

void BasicSound::SetLpfFreq(float lpfFreq) {
    m_LpfFreq = lpfFreq;
}

float BasicSound::GetLpfFreq() const {
    return m_LpfFreq;
}

void BasicSound::SetBiquadFilter(int type, float value) {
    m_BiquadFilterType = static_cast<s8>(type);
    m_BiquadFilterValue = value;
}

void BasicSound::GetBiquadFilter(int* type, float* value) const {
    if (type != nullptr)
        *type = static_cast<int>(m_BiquadFilterType);

    if (value != nullptr)
        *value = m_BiquadFilterValue;
}

void BasicSound::SetOutputLine(u32 lineFlag) {
    m_OutputLineFlag = lineFlag;
}

u32 BasicSound::GetOutputLine() const {
    return m_OutputLineFlag;
}

void BasicSound::ResetOutputLine() {
    m_OutputLineFlag = m_pSoundPlayer->GetDefaultOutputLine();
}

void BasicSound::SetPlayerPriority(int priority) {
    m_Priority = priority;

    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_SortPriorityList(this);

    OnUpdatePlayerPriority();
}

void BasicSound::SetMixMode(MixMode mixMode) {
    m_CommonParam.mixMode = mixMode;
}

MixMode BasicSound::GetMixMode() {
    return m_CommonParam.mixMode;
}

void BasicSound::SetPan(float pan) {
    m_CommonParam.pan = pan;
}

float BasicSound::GetPan() const {
    return m_CommonParam.pan;
}

void BasicSound::SetSurroundPan(float span) {
    m_CommonParam.span = span;
}

float BasicSound::GetSurroundPan() const {
    return m_CommonParam.span;
}

void BasicSound::SetMainSend(float send) {
    m_CommonParam.send[0] = send;
}

float BasicSound::GetMainSend() const {
    return m_CommonParam.send[0];
}

void BasicSound::SetFxSend(AuxBus bus, float send) {
    m_CommonParam.send[1L + bus] = send;
}

float BasicSound::GetFxSend(AuxBus bus) const {
    return m_CommonParam.send[1L + bus];
}

void BasicSound::SetPanMode(PanMode mode) {
    {
        DriverCommand& cmdmgr{*DriverCommand::GetInstance()};
        auto* command{cmdmgr.AllocCommand<DriverCommandPlayerPanParam>()};

        command->id = DriverCommandId_PlayerPanmode;
        command->player = GetBasicSoundPlayerHandle();
        command->panMode = mode;

        cmdmgr.PushCommand(command);
    }
}

void BasicSound::SetPanCurve(PanCurve curve) {
    {
        DriverCommand& cmdmgr{*DriverCommand::GetInstance()};
        auto* command{cmdmgr.AllocCommand<DriverCommandPlayerPanParam>()};

        command->id = DriverCommandId_PlayerPancurve;
        command->player = GetBasicSoundPlayerHandle();
        command->panCurve = curve;

        cmdmgr.PushCommand(command);
    }
}

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
void BasicSound::SetOutputAdditionalParamAddr(OutputDevice device, OutputAdditionalParam* addr,
                                              OutputAdditionalParam* addrForPlayer) {
    m_pOutputAdditionalParam[device] = addr;
    GetBasicSoundPlayerHandle()->SetTvAdditionalParamAddr(addrForPlayer);
}
#endif

void BasicSound::SetOutputVolume(OutputDevice device, float volume) {
    m_OutputParam[device].volume = volume;
}

void BasicSound::SetOutputPan(OutputDevice device, float pan) {
    m_OutputParam[device].pan = pan;
}

void BasicSound::SetOutputSurroundPan(OutputDevice device, float span) {
    m_OutputParam[device].span = span;
}

void BasicSound::SetOutputMainSend(OutputDevice device, float send) {
    m_OutputParam[device].send[OutputDeviceIndex_Main] = send;
}

void BasicSound::SetOutputFxSend(OutputDevice device, AuxBus bus, float send) {
    m_OutputParam[device].send[1L + bus] = send;
}

void BasicSound::SetOutputChannelMixParameter(OutputDevice device, u32 srcChNo,
                                              MixParameter param) {
    for (int i{0}; i < ChannelIndex_Count; ++i)
        m_OutputParam[device].mixParameter[srcChNo].ch[i] = param.ch[i];
}

float BasicSound::GetOutputVolume(OutputDevice device) const {
    return m_OutputParam[device].volume;
}

float BasicSound::GetOutputPan(OutputDevice device) const {
    return m_OutputParam[device].pan;
}

float BasicSound::GetOutputSurroundPan(OutputDevice device) const {
    return m_OutputParam[device].span;
}

float BasicSound::GetOutputMainSend(OutputDevice device) const {
    return m_OutputParam[device].send[OutputDeviceIndex_Main];
}

float BasicSound::GetOutputFxSend(OutputDevice device, AuxBus bus) const {
    return m_OutputParam[device].send[1L + bus];
}

MixParameter BasicSound::GetOutputChannelMixParameter(OutputDevice device, u32 srcChNo) const {
    return m_OutputParam[device].mixParameter[srcChNo];
}

int BasicSound::GetRemainingFadeFrames() const {
    return m_FadeVolume.GetRemainingCount();
}

int BasicSound::GetRemainingPauseFadeFrames() const {
    return m_PauseFadeVolume.GetRemainingCount();
}

int BasicSound::GetRemainingMuteFadeFrames() const {
    return m_MuteFadeVolume.GetRemainingCount();
}

void BasicSound::CalculateSoundParamCalculationValues(
    SoundParamCalculationValues* pOutValue) const {
    pOutValue->soundArchiveParam.volume = m_InitVolume;

    SetSoundPlayerCalculationValues(pOutValue->soundPlayerParam, *m_pSoundPlayer);
    SetSound3DCalculationValues(pOutValue->sound3DParam, m_AmbientParam);
    SetSoundActorCalculationValues(pOutValue->soundActorParam, m_ActorParam);

    pOutValue->soundHandleParam.volume = m_CommonParam.GetVolume();
    pOutValue->soundHandleParam.pitch = m_Pitch;
    pOutValue->soundHandleParam.lpf = m_LpfFreq;
    pOutValue->soundHandleParam.bqfType = static_cast<int>(m_BiquadFilterType);
    pOutValue->soundHandleParam.bqfValue = m_BiquadFilterValue;
    pOutValue->soundHandleParam.outputLineFlag = m_OutputLineFlag;
    pOutValue->soundHandleParam.mixMode = m_CommonParam.mixMode;
    pOutValue->soundHandleParam.pan = m_CommonParam.pan;
    pOutValue->soundHandleParam.surroundPan = m_CommonParam.span;
    pOutValue->soundHandleParam.mainSend = m_CommonParam.send[OutputDeviceIndex_Main];
    pOutValue->soundHandleParam.playerPriority = m_Priority;

    for (int i{0}; i < AuxBus_Count; ++i)
        pOutValue->soundHandleParam.effectSend[i] = m_CommonParam.send[1L + i];

    for (int i{0}; i < OutputDevice_Count; ++i) {
        const OutputDevice device{static_cast<OutputDevice>(i)};

        pOutValue->soundHandleParam.outputVolume[device] = m_OutputParam[device].volume;
        pOutValue->soundHandleParam.outputPan[device] = m_OutputParam[device].pan;
        pOutValue->soundHandleParam.outputSurroundPan[device] = m_OutputParam[device].span;
        pOutValue->soundHandleParam.outputMainSend[device] =
            m_OutputParam[device].send[OutputDeviceIndex_Main];

        for (int k{0}; k < AuxBus_Count; ++k)
            pOutValue->soundHandleParam.outputEffectSend[device][1L + k] =
                m_OutputParam[device].send[1L + k];

        pOutValue->soundHandleParam.outputMixParameter[device][0] =
            m_OutputParam[device].mixParameter[0];
        pOutValue->soundHandleParam.outputMixParameter[device][1] =
            m_OutputParam[device].mixParameter[1];
    }

    pOutValue->resultParam.volume = CalculateVolume();
    pOutValue->resultParam.pitch = CalculatePitch();
    pOutValue->resultParam.lpf = CalculateLpfFrequency();
    pOutValue->resultParam.outputLineFlag = CalculateOutLineFlag();
    pOutValue->resultParam.playerPriority = fnd::Clamp(GetPlayerPriority(), 0, 127);
    CalculateBiquadFilter(&pOutValue->resultParam.bqfType, &pOutValue->resultParam.bqfValue);
    CalculateOutputParam(pOutValue->resultParam.outputParamResult, OutputDevice_Main);

    pOutValue->fadeVolumeParam.stopFadeVolume = m_FadeVolume.GetValue();
    pOutValue->fadeVolumeParam.pauseFadeVolume = m_PauseFadeVolume.GetValue();
    pOutValue->fadeVolumeParam.muteFadeVolume = m_MuteFadeVolume.GetValue();
}

void BasicSound::SetId(u32 id) {
    m_Id = id;
}

bool BasicSound::IsAttachedGeneralHandle() {
    return m_pGeneralHandle != nullptr;
}

bool BasicSound::IsAttachedTempGeneralHandle() {
    return m_pTempGeneralHandle != nullptr;
}

void BasicSound::DetachGeneralHandle() {
    m_pGeneralHandle->DetachSound();
}

void BasicSound::DetachTempGeneralHandle() {
    m_pTempGeneralHandle->DetachSound();
}

float BasicSound::CalculateVolume() const {
    float volume{1.0f};

    if (m_MuteState == MuteState_Muted)
        return 0.0f;

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    if (!IsVolumeThroughModeUsed())
#endif
        volume = m_InitVolume;

    volume *= m_pSoundPlayer->GetVolume();
    volume *= m_CommonParam.GetVolume();
    volume *= m_FadeVolume.GetValue();
    volume *= m_PauseFadeVolume.GetValue();
    volume *= m_MuteFadeVolume.GetValue();
    volume *= m_AmbientParam.GetVolume();
    volume *= m_ActorParam.volume;

    return volume;
}

float BasicSound::CalculatePitch() const {
    return m_Pitch * m_AmbientParam.GetPitch() * m_ActorParam.pitch;
}

}  // namespace nn::atk::detail
