#include <nn/atk/atk_StreamBufferPool.h>

#include <nn/atk/atk_Util.h>
#include "nn/types.h"

namespace nn::atk::detail::driver {
void StreamBufferPool::Initialize(void* buffer, size_t size, int blockCount) {
    if (blockCount == 0)
        return;

    Util::IsValidMemoryForDsp(buffer, size);

    m_Buffer = buffer;
    m_BufferSize = size;
    m_BlockCount = blockCount;
    m_BlockSize = util::align_down(size / blockCount, 64);
    m_AllocCount = 0;
    for (int i {0}; i < BlockMax / BitPerByte; ++i)
        m_AllocFlags[i] = 0;
}

void StreamBufferPool::Finalize() {
    m_Buffer = nullptr;
    m_BufferSize = 0;
    m_BlockSize = 0;
    m_BlockCount = 0;
}

// WIP
void* StreamBufferPool::Alloc() {
    if (m_AllocCount >= m_BlockCount)
        return nullptr;

    const int availableByte {util::align_up(m_BlockCount, BitPerByte)};

    for (int byteIndex {0}; byteIndex < availableByte / BitPerByte; ++byteIndex) {
        if (m_AllocFlags[byteIndex] != 0xff) {
            const u8 byte {m_AllocFlags[byteIndex]};

            u8 mask {1 << 0};
            for (int bitIndex {0}; bitIndex < BitPerByte; ++bitIndex) {
                if ((byte & mask) == 0) {
                    const int totalIndex {bitIndex};
                    void* buffer {util::BytePtr(m_Buffer).Advance(m_BlockSize * (totalIndex | byte)).Get()};
                    return buffer;
                }
            }
        }
    }

    return nullptr;
}

void StreamBufferPool::Free(void* pPtr) {
    ptrdiff_t offset {util::BytePtr(m_Buffer).Distance(pPtr)};

    const u64 totalIndex {offset / m_BlockSize};

    const int mask {1 << (totalIndex & 0b111)};
    const u64 byteIndex {totalIndex};
    const u64 bitIndex {byteIndex / BitPerByte};

    m_AllocFlags[bitIndex] &= ~mask;
    --m_AllocCount;
}
} // namespace nn::atk::detail::driver