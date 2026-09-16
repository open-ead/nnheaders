#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk {

class BiquadFilterCallback {
public:
    using Coefficients = BiquadFilterCoefficients;

    virtual ~BiquadFilterCallback() = default;

    virtual void GetCoefficients(Coefficients* pOutValue, int type, float value) const = 0;
};
static_assert(sizeof(BiquadFilterCallback) == 0x8);

}  // namespace nn::atk
