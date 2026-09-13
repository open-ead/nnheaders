#include <nn/atk/atk_BasicSound.h>

#include <nn/atk/atk_DriverCommand.h>
#include <nn/atk/atk_ExternalSoundPlayer.h>
#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/atk_SoundPlayer.h>

namespace nn::atk::detail {

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

    for (int i{0}; i < 1; ++i) {
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

void BasicSound::SetPlayerPriority(int priority) {
    m_Priority = priority;

    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_SortPriorityList(this);

    OnUpdatePlayerPriority();
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

}  // namespace nn::atk::detail
