#include <nn/atk/atk_StreamBufferPool.h>

#include <nn/atk/atk_Util.h>

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
} // namespace nn::atk::detail::driver