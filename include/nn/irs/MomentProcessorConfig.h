#pragma once

#include <nn/irs/Rect.h>
#include <nn/types.h>

namespace nn::irsensor {
struct MomentProcessorConfig {
    s64 exposureTime = 300000;
    s32 lightTarget = 0;
    s32 gain = 8;
    bool isNegativeImageUsed = false;
    u8 reserved[7];
    Rect windowOfInterest;
    s32 preprocess = 1;
    s32 preprocessIntensityThreshold = 80;
};
}  // namespace nn::irsensor
