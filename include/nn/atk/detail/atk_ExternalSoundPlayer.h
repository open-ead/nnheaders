#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/atk_BasicSound.h>

namespace nn::atk::detail {
class ExternalSoundPlayer {
public:
    using SoundList = util::IntrusiveList<
                        BasicSound, util::IntrusiveListMemberNodeTraits<
                            BasicSound, &BasicSound::m_ExtSoundPlayerPlayLink>>;

    virtual ~ExternalSoundPlayer();
    virtual bool CanPlaySound(s32);

    ExternalSoundPlayer();

    void StopAllSound(s32 fadeFrames);
    void PauseAllSound(bool, s32);
    void PauseAllSound(bool, s32, PauseMode);

    void Finalize(SoundActor* actor);

    void RemoveSound(BasicSound* sound);
    bool AppendSound(BasicSound* sound);

    BasicSound* GetLowestPrioritySound();

    void SetPlayableSoundCount(s32 count);

private:
    SoundList m_SoundList;
    s32 m_PlayableCount;
};
static_assert(sizeof(ExternalSoundPlayer) == 0x20);
} // namespace nn::atk::detail