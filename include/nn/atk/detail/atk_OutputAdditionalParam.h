#pragma once

#include <nn/types.h>

#include <nn/atk/detail/atk_ValueArray.h>
#include <nn/atk/detail/atk_BusMixVolumePacket.h>
#include <nn/atk/detail/atk_VolumeThroughModePacket.h>

namespace nn::atk::detail {
using SendArray = ValueArray<f32>;

class OutputAdditionalParam {
public:
    static std::size_t GetRequiredMemSize(SoundInstanceConfig* config);

    void Initialize(void* buffer, std::size_t bufferSize, SoundInstanceConfig* config);

    void Reset();

    void* GetBufferAddr();

    SendArray* GetAdditionalSendAddr();
    f32 TryGetAdditionalSend(s32 bus);
    bool IsAdditionalSendEnabled();
    void TrySetAdditionalSend(s32 bus, f32 send);

    BusMixVolumePacket* GetBusMixVolumePacketAddr();
    f32 GetBusMixVolume(s32 waveChannel, s32 mixChannel);
    OutputBusMixVolume* GetBusMixVolume();
    void SetBusMixVolume(OutputBusMixVolume* param);
    bool IsBusMixVolumeUsed();
    void SetBusMixVolumeUsed(bool isUsed);
    bool IsBusMixVolumeEnabledForBus(s32 bus);
    void SetBusMixVolumeEnabledForBus(s32 bus, bool isEnabled);
    bool IsBusMixVolumeEnabled();

    VolumeThroughModePacket* GetVolumeThroughModePacketAddr();
    f32 GetBinaryVolume();
    void SetBinaryVolume(f32 volume);
    u8 TryGetVolumeThroughMode(s32 bus);
    bool IsVolumeThroughModeEnabled();
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