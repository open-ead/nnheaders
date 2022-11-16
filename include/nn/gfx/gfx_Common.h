#pragma once

#include <nn/types.h>

// not sure what these are named
#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>

// todo: figure out where to place this?
#define NN_GFX_NO_COPY(CLASS)                                                                      \
private:                                                                                           \
    CLASS(const CLASS&) = delete;                                                                  \
    CLASS& operator=(const CLASS&) = delete;

namespace nn::gfx {

struct OutOfMemoryEventArg {
    size_t minRequiredSize;
};

union ClearColorValue {
    float valueFloat[4];
    int32_t valueInt[4];
    uint32_t valueUint[4];
};

}  // namespace nn::gfx