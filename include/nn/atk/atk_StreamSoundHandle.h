#pragma once

#include <nn/util/util_BitFlagSet.h>

#include <nn/atk/atk_SoundHandle.h>

namespace nn::atk {
namespace detail {
class StreamSound;
} // namespace nn::atk::detail

class StreamSoundHandle {
public:
    using TrackBitFlagSet = util::BitFlagSet<8, void>;

    explicit StreamSoundHandle(SoundHandle* pSoundHandle);

    void detail_AttachSoundAsTempHandle(detail::StreamSound* pSound);
    void DetachSound();

private:
    detail::StreamSound* m_pSound;
};
} // namespace nn::atk