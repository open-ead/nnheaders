#include <nn/atk/atk_BasicSound.h>

#include <nn/atk/atk_SoundPlayer.h>

namespace nn::atk::detail {

// NON_MATCHING on versions lower than 4.0.0
BasicSound::BasicSound() = default;

void BasicSound::StartPrepared() {
    m_StartFlag = true;
}

void BasicSound::SetPlayerPriority(int priority) {
    m_Priority = priority;
    
    if (m_pSoundPlayer != nullptr)
        m_pSoundPlayer->detail_SortPriorityList(this);
    
    OnUpdatePlayerPriority();
}
}  // namespace nn::atk::detail