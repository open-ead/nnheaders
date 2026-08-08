#pragma once

#include <nn/types.h>

namespace nn::audio {
struct WaveBuffer {
    const void* buffer;
    size_t size;
    s32 startSampleOffset;
    s32 endSampleOffset;
    bool loop;
    bool isEndOfStream;
    const void* pContext;
    size_t contextSize;
};
static_assert(sizeof(WaveBuffer) == 0x30);
} // namespace nn::audio