#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk {
class ChannelMixVolume {
public:
    constexpr static u8 ChannelCountMax = 24; 

    ChannelMixVolume();
    
    void InitializeChannelVolume();

    explicit ChannelMixVolume(const MixVolume& mixVolume);
    ChannelMixVolume(const f32* channelVolumes, s32 channelCount);

    bool SetChannelCount(s32 channelCount);
    
    bool SetChannelVolume(s32 channel, f32 volume);
    f32 GetChannelVolume(s32 channel) const;

    bool SetChannelVolume(s32 channel, const f32*, s32);

private:
    s32 m_ChannelCount;
    f32 m_ChannelVolume[ChannelCountMax];
};
static_assert(sizeof(ChannelMixVolume) == 0x64);
} // namespace nn::atk