#pragma once

#include <nn/gfx/gfx_Enum.h>

namespace nn::gfx::detail {

int GetBlockWidth(ChannelFormat);
int GetBlockHeight(ChannelFormat);
bool IsCompressedFormat(ChannelFormat);
int GetBytePerPixel(ChannelFormat);

inline ChannelFormat GetChannelFormat(ImageFormat format) {
    return static_cast<ChannelFormat>(format >> 8);
}

}  // namespace nn::gfx::detail