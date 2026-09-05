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

size_t MemoryFileStream::Read(void* buf, size_t length, [[maybe_unused]] fnd::FndResult* result) {
    size_t readLen{std::min(length, m_Size - m_Position)};

    std::memcpy(buf, reinterpret_cast<const char*>(m_pBuffer) + m_Position, readLen);

    m_Position += static_cast<position_t>(readLen);

    return readLen;
}

fnd::FndResult MemoryFileStream::Seek(position_t offset, fnd::Stream::SeekOrigin origin) {
    switch (origin) {
    case SeekOrigin_Begin:
        m_Position = offset;
        break;

    case SeekOrigin_End:
        m_Position = static_cast<position_t>(m_Size) - offset;
        break;

    case SeekOrigin_Current:
        m_Position += offset;
        break;

    default:
        return fnd::FndResult{fnd::FndResultType_Failed};
    }

    return fnd::FndResult{fnd::FndResultType_True};
}

}  // namespace nn::atk::detail