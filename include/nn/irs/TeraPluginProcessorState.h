#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct TeraPluginProcessorState {
    s64 samplingNumber;
    s64 timeStamp;
    s32 ambientNoiseLevel;
    u8 pluginData[0x12c];
};
}  // namespace nn::irsensor
