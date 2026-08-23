#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk::detail {
struct BusMixVolumePacket {
public:
    static size_t GetRequiredMemSize(int busCount);

    BusMixVolumePacket();
    
    bool Initialize(void* buffer, size_t size, int busCount);
    void Finalize();

    void Reset();

    int GetBusCount() const { 
        return m_BusCount; 
    }

    float GetBusMixVolume(int waveChannel, int mixChannel) const {
        return m_BusMixVolume.volume[waveChannel][mixChannel];
    }

    const OutputBusMixVolume& GetBusMixVolume() const {
        return m_BusMixVolume;
    }

    void SetBusMixVolume(int waveChannel, int mixChannel, float volume) {
        m_BusMixVolume.volume[waveChannel][mixChannel] = volume;
    }

    void SetBusMixVolume(const OutputBusMixVolume& busMixVolume) {
        m_BusMixVolume = busMixVolume;
    }

    bool IsUsed() const {
        return m_IsUsed;
    }

    void SetUsed(bool isUsed) {
        m_IsUsed = isUsed;
    }

    bool IsEnabled(int bus) const {
        return m_pIsEnabledTable[bus];
    }

    void SetEnabled(int bus, bool isEnabled) {
        m_pIsEnabledTable[bus] = isEnabled;
    }

private:
    OutputBusMixVolume m_BusMixVolume;
    bool m_IsUsed;
    bool* m_pIsEnabledTable;
    int m_BusCount;
};
static_assert(sizeof(BusMixVolumePacket) == 0xd8);
} // namespace nn::atk::detail