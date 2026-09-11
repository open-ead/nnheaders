#include <nn/atk/atk_BasicSound.h>

#include <nn/atk/atk_SoundPlayer.h>

namespace nn::atk::detail {

// NON_MATCHING on versions lower than 4.0.0
BasicSound::BasicSound() = default;

void BasicSound::StartPrepared() {
    m_StartFlag = true;
}

void BasicSound::Stop(int fadeFrames) {
    if ((fadeFrames <= 0 || m_PauseState == PauseState_Paused) || (!m_StartFlag && !m_StartedFlag)) {
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

void BasicSound::SetPlayerPriority(int priority) {
    m_Priority = priority;
    
    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_SortPriorityList(this);
    
    OnUpdatePlayerPriority();
}
}  // namespace nn::atk::detail