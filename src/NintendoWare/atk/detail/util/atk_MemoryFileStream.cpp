#include <nn/atk/detail/atk_MemoryFileStream.h>

#include <algorithm>

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

size_t MemoryFileStream::Read(void* buf, size_t length, fnd::FndResult* result) {
    size_t readLen{std::min(length, m_Size - m_Position)};

    std::memcpy(buf, reinterpret_cast<const char*>(m_pBuffer) + m_Position, readLen);

    m_Position += readLen;

    return readLen;
}

}  // namespace nn::atk::detail