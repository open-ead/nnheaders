#pragma once

#include <nn/irs/Rect.h>
#include <nn/types.h>

namespace nn::irsensor {
struct ClusteringProcessorConfig {
    s64 exposureTime = 200000;
    s32 lightTarget = 0;
    s32 gain = 2;
    bool isNegativeImageUsed = false;
    u8 reserved[7];
    Rect windowOfInterest;
    s32 objectPixelCountMin = 3;
    s32 objectPixelCountMax = 76800;  // windowOfInterest.width * windowOfInterest.height;
    s32 objectIntensityMin = 150;
    bool isExternalLightFilterEnabled = true;
};
}  // namespace nn::irsensor
