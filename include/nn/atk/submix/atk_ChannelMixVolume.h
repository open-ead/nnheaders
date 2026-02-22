#pragma once

#include <nn/types.h>

namespace nn::atk {
class ChannelMixVolume {
public:
private:
    s32 m_ChannelCount;
    f32 m_ChannelVolume[24];
};
} // namespace nn::atk