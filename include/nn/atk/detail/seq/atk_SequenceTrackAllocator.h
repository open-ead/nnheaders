#pragma once

#include <nn/atk/detail/seq/atk_SequenceTrack.h>
#include <nn/atk/detail/seq/atk_SequenceSoundPlayer.h>

namespace nn::atk::detail::driver {
class SequenceTrackAllocator {
public:
    virtual ~SequenceTrackAllocator() = default;
    virtual SequenceTrack* AllocTrack(SequenceSoundPlayer* player) = 0;
};
static_assert(sizeof(SequenceTrackAllocator) == 0x8);
} // namespace nn::atk::detail::driver