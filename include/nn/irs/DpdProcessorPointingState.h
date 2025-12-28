#pragma once

#include <nn/irs/Rect.h>
#include <nn/types.h>

namespace nn::irsensor {
struct DpdProcessorPointingState {
    s64 samplingNumber;
    s64 timeStamp;
    struct {
        u8 pointingStatus;
        s32 _4;
        f32 _8;
        f32 positionX;
        f32 positionY;
        f32 _14;
        Rect windowOfInterest;
    } data[3];
};
}  // namespace nn::irsensor
