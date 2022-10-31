#pragma once

#include <nn/types.h>

// not sure what these are named
#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>

namespace nn::gfx {

struct OutOfMemoryEventArg {
    size_t minRequiredSize;
};

}  // namespace nn::gfx