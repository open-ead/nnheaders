#pragma once

#include <nn/util/util_BitFlagSet.h>

#include <nn/atk/atk_SoundHandle.h>

namespace nn::atk {
namespace detail {
class WaveSound;
} // namespace nn::atk::detail

class WaveSoundHandle {
public:
    using TrackBitFlagSet = util::BitFlagSet<8, void>;

    explicit WaveSoundHandle(SoundHandle* pSoundHandle);

    void detail_AttachSoundAsTempHandle(detail::WaveSound* pSound);

    void ForceStop();

    void DetachSound();

private:
    detail::WaveSound* m_pSound;
};
} // namespace nn::atk