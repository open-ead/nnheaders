#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct TeraPluginProcessorState {
    s64 samplingNumber;
    s64 timeStamp;
    s32 ambientNoiseLevel;
    u8 data[0x12c];  // TODO: this is unknown
};
}  // namespace nn::irsensor
