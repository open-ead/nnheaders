#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
class PoolImpl {
public:
    s32 CreateImpl(void* buffer, std::size_t size, std::size_t objSize);
    s32 CreateImpl(void* buffer, std::size_t size, std::size_t objSize, 
                   std::size_t alignment);

    void DestroyImpl();

    s32 CountImpl() const;

    void* AllocImpl();

    void FreeImpl(void* ptr);

private:
    PoolImpl* m_pNext;
    void* m_pBuffer;
    std::size_t m_BufferSize;
};
static_assert(sizeof(PoolImpl) == 0x18);

class BufferPool : PoolImpl {};

template <typename T>
class InstancePool : PoolImpl {};
} // namespace nn::atk::detail