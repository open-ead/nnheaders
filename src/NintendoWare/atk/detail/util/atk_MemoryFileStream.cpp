#include <nn/atk/detail/atk_MemoryFileStream.h>

namespace nn::atk::detail {

MemoryFileStream::MemoryFileStream(const void* buffer, size_t size)
    : m_pBuffer(buffer), m_Size(size), m_Position(0) {}

fnd::FndResult MemoryFileStream::Open(const char* filePath, AccessMode openMode) {
    return fnd::FndResult{fnd::FndResultType_Failed};
}

}  // namespace nn::atk::detail