#pragma once

#include <nn/types.h>

namespace nn::audio {
struct AdpcmContext {
    u16 predScale;
    s16 history[2];
};
static_assert(sizeof(AdpcmContext) == 0x6);

struct AdpcmParameter {
    u16 coefficients[16];
};
static_assert(sizeof(AdpcmParameter) == 0x20);
} // namespace nn::audio