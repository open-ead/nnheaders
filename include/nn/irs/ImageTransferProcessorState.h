#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct ImageTransferProcessorState {
    s64 samplingNumber;
    s32 ambientNoiseLevel;
    u8 reserved[4];
};
}  // namespace nn::irsensor
