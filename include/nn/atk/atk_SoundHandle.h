#pragma once

#include <nn/atk/detail/atk_BasicSound.h>

namespace nn::atk {
class SoundHandle {
public:
    enum MuteState {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting,
        MuteState_Invalid,
    };

    enum PauseState {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing,
        PauseState_Invalid
    };

    ~SoundHandle() = default;

    void DetachSound();

    void detail_DuplicateHandle(SoundHandle* other);

    void detail_AttachSound(detail::BasicSound* sound);
    void detail_AttachSoundAsTempHandle(detail::BasicSound* sound);

    void CalculateSoundParamCalculationValues(SoundParamCalculationValues* calcValues) const;

private:
    detail::BasicSound* m_pSound;
};
static_assert(sizeof(SoundHandle) == 0x8);
} // namespace nn::atk