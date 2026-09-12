#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {
class WorkBufferAllocator {
public:
    WorkBufferAllocator(void* buffer, size_t size);

    void* Allocate(size_t size, size_t alignment);
    void* Allocate(size_t size, size_t alignment, s32 count);

private:
    std::uintptr_t m_Buffer;
    size_t m_Offset;
    size_t m_Size;
};
static_assert(sizeof(WorkBufferAllocator) == 0x18);
} // namespace nn::atk::detail::fnd