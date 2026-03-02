#pragma once

#include <nn/atk/util/atk_Global.h>

namespace nn::atk {
class BiquadFilterCallback {
public:
    using Coefficients = BiquadFilterCoefficients;

    virtual ~BiquadFilterCallback();

    virtual void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) = 0;
};
static_assert(sizeof(BiquadFilterCallback) == 0x8);
}