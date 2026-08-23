#include <nn/atk/atk_BusMixVolumePacket.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {
BusMixVolumePacket::BusMixVolumePacket() = default;

size_t BusMixVolumePacket::GetRequiredMemSize(int busCount) {
    size_t result {util::align_up<u64>(busCount, 8)};
    return result;
}
} // namespace nn::atk::detail