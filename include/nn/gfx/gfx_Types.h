#pragma once

#include <nn/gfx/gfx_Common.h>

namespace nn::gfx {

typedef TCommandBuffer<DefaultApi> CommandBuffer;

typedef TMemoryPool<DefaultApi> MemoryPool;

typedef TDevice<DefaultApi> Device;

}  // namespace nn::gfx