#pragma once

#include <nn/types.h>

namespace nn {

typedef void* (*AlignedAllocateFunctionWithUserData)(size_t, size_t, void*);
typedef void (*FreeFunctionWithUserData)(void*, void*);

}  // namespace nn
