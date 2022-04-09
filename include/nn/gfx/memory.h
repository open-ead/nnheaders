/**
 * @file memory.h
 * @brief GFX Memory Pool.
 */

#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/pool.h>

namespace nn::gfx {

// todo: finish me!
template <typename Api>
class TMemoryPool : public detail::MemoryPoolImpl<Api> {};

using MemoryPool = TMemoryPool<DefaultApi>;

}  // namespace nn::gfx
