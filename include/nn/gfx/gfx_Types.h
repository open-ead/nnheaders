#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>

namespace nn::gfx {

typedef TCommandBuffer<DefaultApi> CommandBuffer;

typedef TMemoryPool<DefaultApi> MemoryPool;

}  // namespace nn::gfx