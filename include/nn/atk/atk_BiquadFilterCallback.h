#pragma once

#include <nn/types.h>

#include <nn/atk/atk_Global.h>

namespace nn::atk {
class BiquadFilterCallback {
public:
    typedef BiquadFilterCoefficients Coefficients;

    virtual ~BiquadFilterCallback();
};

}