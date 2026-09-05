#include <nn/atk/detail/atk_MemoryFileStream.h>

namespace nn::atk::detail {

MemoryFileStream::MemoryFileStream(const void* buffer, size_t size)
    : m_pBuffer(buffer), m_Size(size), m_Position(0) {}

}  // namespace nn::atk::detail