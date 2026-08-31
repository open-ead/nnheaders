#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk {
class ChannelMixVolume {
public:
    static const int ChannelCountMax = 24;

    ChannelMixVolume();
    explicit ChannelMixVolume(const MixVolume& mixVolume);
    ChannelMixVolume(const float* pVolume, int volumeCount);
    
    bool SetChannelCount(int channelCount);
    int GetChannelCount() const;

    bool SetChannelVolume(int channelIndex, float volume);
    float GetChannelVolume(int channelIndex) const;

    bool SetChannelVolume(int channelIndex, const float* pVolume, int volumeCount);

    void InitializeChannelVolume();

private:
    int m_ChannelCount;
    float m_ChannelVolume[ChannelCountMax];
};
static_assert(sizeof(ChannelMixVolume) == 0x64);
} // namespace nn::atk