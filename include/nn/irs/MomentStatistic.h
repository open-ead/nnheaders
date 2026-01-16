#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct MomentStatistic {
    f32 averageIntensity;
    f32 centroidX;
    f32 centroidY;
};
}  // namespace nn::irsensor
