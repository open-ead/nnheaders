#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk::detail {
class BusMixVolumePacket {
public:
    static std::size_t GetRequiredMemSize(s32 busCount);

    bool Initialize(void* buffer, std::size_t size, s32 busCount);

    void Reset();

private:
    OutputBusMixVolume m_BusMixVolume;
    bool m_IsUsed;
    bool* m_pIsEnabledTable;
    s32 m_BusCount;
};
static_assert(sizeof(BusMixVolumePacket) == 0xd8);
} // namespace nn::atk::detail