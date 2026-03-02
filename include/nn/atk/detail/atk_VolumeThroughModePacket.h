#pragma once

#include <nn/atk/detail/atk_ValueArray.h>

namespace nn::atk::detail {
class VolumeThroughModePacket {
public:
    using VolumeThroughModeArray = ValueArray<u8>;

    static std::size_t GetRequiredMemSize(s32 busCount);

    bool Initialize(void* buffer, std::size_t size, s32 busCount);

    void Finalize();

    void Reset();

    VolumeThroughModePacket& operator=(const VolumeThroughModePacket& rhs);

private:
    VolumeThroughModeArray m_VolumeThroughMode;
    bool m_IsVolumeThroughModeUsed;
    f32 m_BinaryVolume;
};
static_assert(sizeof(VolumeThroughModePacket) == 0x18);
} // namespace nn::atk::detail