#pragma once

#include <nn/irs/MomentStatistic.h>
#include <nn/irs/Rect.h>

namespace nn::irsensor {
struct ClusteringData {
    MomentStatistic momentStatistic;
    s32 pixelCount;
    Rect bounds;
};
}  // namespace nn::irsensor
