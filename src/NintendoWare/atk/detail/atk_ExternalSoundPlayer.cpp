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

}  // namespace nn::atk::detail
