#pragma once

#include <nn/atk/detail/seq/atk_SequenceTrackAllocator.h>
#include <nn/atk/detail/seq/atk_MmlParser.h>
#include <nn/atk/detail/util/atk_InstancePool.h>

namespace nn::atk::detail::driver {
class MmlSequenceTrackAllocator : SequenceTrackAllocator {
public:
    using MmlSequenceTrackPool = InstancePool<MmlSequenceTrack>;

    SequenceTrack* AllocTrack(SequenceSoundPlayer* player) override;

    void FreeTrack(SequenceTrack* track);

    s32 Create(void* buffer, std::size_t size);

    void Destroy();

    s32 GetAllocatableTrackCount();

private:
    MmlParser* m_pParser;
    MmlSequenceTrackPool m_TrackPool;
};
static_assert(sizeof(MmlSequenceTrackAllocator) == 0x28);
} // namespace nn::atk::detail