#pragma once

#include <nn/types.h>

#include <nn/atk/detail/atk_ValueArray.h>
#include <nn/atk/detail/atk_BusMixVolumePacket.h>
#include <nn/atk/detail/atk_VolumeThroughModePacket.h>

namespace nn::atk::detail {
using SendArray = ValueArray<f32>;

class OutputAdditionalParam {
public:
    static std::size_t GetRequiredMemSize(const SoundInstanceConfig& config);

    void Initialize(void* buffer, std::size_t bufferSize, const SoundInstanceConfig& config);
    void Finalize();

    void Reset();

    void* GetBufferAddr();

    SendArray* GetAdditionalSendAddr();
    SendArray* GetAdditionalSendAddr() const;

    f32 TryGetAdditionalSend(s32 bus) const;

    bool IsAdditionalSendEnabled() const;

    void TrySetAdditionalSend(s32 bus, f32 send);

    BusMixVolumePacket* GetBusMixVolumePacketAddr();
    BusMixVolumePacket* GetBusMixVolumePacketAddr() const;

    f32 GetBusMixVolume(s32 waveChannel, s32 mixChannel) const;
    OutputBusMixVolume* GetBusMixVolume() const;

    void SetBusMixVolume(s32 waveChannel, s32 mixChannel, f32 volume);
    void SetBusMixVolume(const OutputBusMixVolume& param);

    bool IsBusMixVolumeUsed() const;
    void SetBusMixVolumeUsed(bool isUsed);

    bool IsBusMixVolumeEnabledForBus(s32 bus) const;
    void SetBusMixVolumeEnabledForBus(s32 bus, bool isEnabled);

    bool IsBusMixVolumeEnabled() const;

    VolumeThroughModePacket* GetVolumeThroughModePacketAddr();
    VolumeThroughModePacket* GetVolumeThroughModePacketAddr() const;

    f32 GetBinaryVolume() const;
    void SetBinaryVolume(f32 volume);

    u8 TryGetVolumeThroughMode(s32 bus) const;
    void TrySetVolumeThroughMode(s32 bus, u8 volumeThroughMode);
    bool IsVolumeThroughModeEnabled() const;
    
    bool IsVolumeThroughModeUsed();
    void SetVolumeThroughModeUsed(bool isUsed);

    OutputAdditionalParam& operator=(const OutputAdditionalParam& rhs);

private:
    SendArray* m_pAdditionalSend;
    BusMixVolumePacket* m_pBusMixVolumePacket;
    VolumeThroughModePacket* VolumeThroughModePacket;
};
static_assert(sizeof(OutputAdditionalParam) == 0x18);
} // namespace nn::atk::detail