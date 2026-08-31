#pragma once

#include <nn/types.h>

namespace nn::atk::detail::driver {
class StreamBufferPool {
public:
    void Initialize(void* buffer, size_t size, int blockCount);
    void Finalize();

    void* Alloc();
    void Free(void* pPtr);

    size_t GetBlockSize() const { return m_BlockSize; }

private:
    static const int BlockMax   {32};
    static const int BitPerByte  {8};

    void* m_Buffer;
    size_t m_BufferSize;
    size_t m_BlockSize;
    int m_BlockCount;
    int m_AllocCount;
    u8 m_AllocFlags[BlockMax / BitPerByte];
};
static_assert(sizeof(StreamBufferPool) == 0x28);
} // namespace nn::atk::detail