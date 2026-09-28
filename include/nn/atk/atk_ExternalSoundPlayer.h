#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_BasicSound.h>

namespace nn::atk::detail {

class ExternalSoundPlayer {
public:
    using SoundList = util::IntrusiveList<
        BasicSound,
        util::IntrusiveListMemberNodeTraits<BasicSound, &BasicSound::m_ExtSoundPlayerPlayLink>>;

    ExternalSoundPlayer();
    virtual ~ExternalSoundPlayer();

    void StopAllSound(int fadeFrames);

    void PauseAllSound(bool flag, int fadeFrames);
    void PauseAllSound(bool flag, int fadeFrames, PauseMode pauseMode);

    int GetPlayingSoundCount() const { return m_SoundList.size(); }

    void SetPlayableSoundCount(int count);
    int GetPlayableSoundCount() const { return m_PlayableCount; }

    virtual bool CanPlaySound(int startPriority);

    bool AppendSound(BasicSound* sound);
    void RemoveSound(BasicSound* sound);

    void Finalize(SoundActor* actor);

protected:
    BasicSound* GetLowestPrioritySound();

private:
    SoundList m_SoundList;
    int m_PlayableCount{1};
};
static_assert(sizeof(ExternalSoundPlayer) == 0x20);

}  // namespace nn::atk::detail
