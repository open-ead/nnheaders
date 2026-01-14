#pragma once

#include <nn/irs/MomentStatistic.h>
#include <nn/types.h>

namespace nn::irsensor {
struct MomentProcessorState {
    s64 samplingNumber;
    s64 timeStamp;
    s32 ambientNoiseLevel;
    u8 reserved[4];
    MomentStatistic statistics[48];
};
}  // namespace nn::irsensor
