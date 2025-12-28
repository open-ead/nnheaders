#pragma once

#include <nn/irs/ClusteringData.h>
#include <nn/types.h>

namespace nn::irsensor {
struct ClusteringProcessorState {
    s64 samplingNumber;
    s64 timeStamp;
    u8 objectCount;
    s32 ambientNoiseLevel;
    ClusteringData data[16];
};
}  // namespace nn::irsensor
