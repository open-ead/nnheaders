#pragma once

#include <nn/util/util_BitFlagSet.h>

#include <nn/atk/atk_SoundHandle.h>

namespace nn::atk {
namespace detail {
class SequenceSound;
} // namespace nn::atk::detail

class SequenceSoundHandle {
public:
    using TrackBitFlagSet = util::BitFlagSet<16, void>;

    constexpr static u32 BankIndexMin = 0;
    constexpr static u32 BankIndexMax = 3;

    constexpr static u8 TransposeMin = 192;
    constexpr static u8 TransposeMax = 63;

    constexpr static u8 VelocityRangeMin = 0;
    constexpr static u8 VelocityRangeMax = 127;

    constexpr static u32 VariableIndexMax = 15;
    constexpr static u32 TrackIndexMax = 15;

    explicit SequenceSoundHandle(SoundHandle* pSoundHandle);

    void detail_AttachSoundAsTempHandle(detail::SequenceSound* pSound);
    void DetachSound();

private:
    detail::SequenceSound* m_pSound;
};
} // namespace nn::atk