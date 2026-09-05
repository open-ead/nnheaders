#include <nn/atk/detail/atk_MemoryFileStream.h>

namespace nn::atk::detail {

MemoryFileStream::MemoryFileStream(const void* buffer, size_t size)
    : m_pBuffer(buffer), m_Size(size), m_Position(0) {}

fnd::FndResult MemoryFileStream::Open([[maybe_unused]] const char* filePath,
                                      [[maybe_unused]] AccessMode openMode) {
    return fnd::FndResult{fnd::FndResultType_Failed};
}

void MemoryFileStream::Close() {
    m_pBuffer = nullptr;
    m_Size = 0;
    m_Position = 0;
}

void MemoryFileStream::Flush() {}

}  // namespace nn::atk::detail