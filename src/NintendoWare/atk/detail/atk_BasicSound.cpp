#include <nn/atk/atk_BasicSound.h>

#include <nn/atk/atk_SoundPlayer.h>

namespace nn::atk::detail {

// NON_MATCHING on versions lower than 4.0.0
BasicSound::BasicSound() = default;

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

void BasicSound::SetPlayerPriority(int priority) {
    m_Priority = priority;

    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_SortPriorityList(this);

    OnUpdatePlayerPriority();
}

}  // namespace nn::atk::detail
