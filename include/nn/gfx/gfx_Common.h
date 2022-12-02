#pragma once

#include <nn/types.h>

// not sure what these are named
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/gfx_VariationBase.h>
#include <nn/util.h>

namespace nn::gfx {

// todo: figure out where to put this
enum LowLevelApi { LowLevelApi_Common = 0 };

// todo: seems to be named ApiVariationNvn8
using NvnApi = ApiVariation<ApiType<4>, ApiVersion<8>>;
using DefaultApi = NvnApi;

struct OutOfMemoryEventArg {
    size_t minRequiredSize;
};

union ClearColorValue {
    float valueFloat[4];
    int32_t valueInt[4];
    uint32_t valueUint[4];
};

}  // namespace nn::gfx