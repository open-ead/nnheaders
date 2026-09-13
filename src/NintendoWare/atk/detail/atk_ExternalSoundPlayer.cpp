#include <nn/atk/atk_ExternalSoundPlayer.h>

namespace nn::atk::detail {

ExternalSoundPlayer::ExternalSoundPlayer() = default;

ExternalSoundPlayer::~ExternalSoundPlayer() {
    for (auto itr{m_SoundList.begin()}; itr != m_SoundList.end();) {
        auto curItr{itr++};
        curItr->DetachExternalSoundPlayer(this);
    }
}

void ExternalSoundPlayer::StopAllSound(int fadeFrames) {
    for (auto itr{m_SoundList.begin()}; itr != m_SoundList.end();) {
        auto curItr{itr++};
        curItr->Stop(fadeFrames);
    }
}

void ExternalSoundPlayer::PauseAllSound(bool flag, int fadeFrames) {
    for (auto itr{m_SoundList.begin()}; itr != m_SoundList.end();) {
        auto curItr{itr++};
        curItr->Pause(flag, fadeFrames);
    }
}

void ExternalSoundPlayer::PauseAllSound(bool flag, int fadeFrames, PauseMode pauseMode) {
    for (auto itr{m_SoundList.begin()}; itr != m_SoundList.end();) {
        auto curItr{itr++};
        curItr->Pause(flag, fadeFrames, pauseMode);
    }
}

}  // namespace nn::atk::detail
