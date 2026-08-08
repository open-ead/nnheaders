#pragma once

#include <nn/types.h>

namespace nn::atk::detail::driver {
class StreamBufferPool {
public:
    constexpr static u32 BlockMax = 32;
    constexpr static u8 BitPerByte = 8;

    void Initialize(void* buffer, size_t size, s32 blockCount);
    void Finalize();

    void* Alloc();
    void Free(void* pPtr);

private:
    void* m_Buffer;
    size_t m_BufferSize;
    size_t m_BlockSize;
    s32 m_BlockCount;
    s32 m_AllocCount;
    u8 m_AllocFlags[BlockMax / BitPerByte];
};
static_assert(sizeof(StreamBufferPool) == 0x28);
} // namespace nn::atk::detail