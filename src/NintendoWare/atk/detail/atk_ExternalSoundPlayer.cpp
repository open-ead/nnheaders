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

void ExternalSoundPlayer::SetPlayableSoundCount(int count) {
    m_PlayableCount = count;

    while (GetPlayingSoundCount() > GetPlayableSoundCount()) {
        BasicSound* dropSound{GetLowestPrioritySound()};
        dropSound->Finalize();
    }
}

bool ExternalSoundPlayer::CanPlaySound(int startPriority) {
    if (GetPlayableSoundCount() == 0)
        return false;

    if (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        BasicSound* dropSound{GetLowestPrioritySound()};

        if (dropSound == nullptr)
            return false;

        if (startPriority < dropSound->CalcCurrentPlayerPriority())
            return false;
    }

    return true;
}

void ExternalSoundPlayer::RemoveSound(BasicSound* sound) {
    m_SoundList.erase(m_SoundList.iterator_to(*sound));
    sound->DetachExternalSoundPlayer(this);
}

BasicSound* ExternalSoundPlayer::GetLowestPrioritySound() {
    if (m_SoundList.empty())
        return nullptr;

    int priority{PlayerPriorityMax + 1};
    BasicSound* sound{};

    for (auto itr{m_SoundList.begin()}; itr != m_SoundList.end(); ++itr) {
        int itrPriority{itr->CalcCurrentPlayerPriority()};

        sound = priority > itrPriority ? &*itr : sound;
        priority = priority > itrPriority ? itrPriority : priority;
    }

    return sound;
}

}  // namespace nn::atk::detail
