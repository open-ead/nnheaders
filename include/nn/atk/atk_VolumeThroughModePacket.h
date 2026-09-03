#pragma once

#include <nn/atk/atk_ValueArray.h>

namespace nn::atk::detail {
class VolumeThroughModePacket {
public:
    using VolumeThroughModeArray = ValueArray<u8>;

    VolumeThroughModePacket();

    static size_t GetRequiredMemSize(int busCount);

    bool Initialize(void* buffer, size_t size, int busCount);
    void Finalize();
    void Reset();

    int GetBusCount() const { return m_VolumeThroughMode.GetCount(); }

    VolumeThroughModePacket& operator=(const VolumeThroughModePacket& rhs);

private:
    VolumeThroughModeArray m_VolumeThroughMode;
    bool m_IsVolumeThroughModeUsed;
    float m_BinaryVolume;
};
static_assert(sizeof(VolumeThroughModePacket) == 0x18);
}  // namespace nn::atk::detail