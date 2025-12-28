#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct DpdProcessorState {
    s64 samplingNumber;
    s64 timeStamp;
    s32 pointingStatus;
    f32 positionX;
    f32 positionY;
};
}  // namespace nn::irsensor
