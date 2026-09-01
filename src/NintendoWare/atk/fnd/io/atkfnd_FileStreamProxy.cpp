#include <nn/atk/fnd/io/atkfnd_FileStreamProxy.h>

namespace nn::atk::detail::fnd {
FileStreamProxy::FileStreamProxy(FileStream* fileStream, position_t offset, size_t fileSize)
    : m_pFileStream{fileStream}, m_Offset{offset}, m_FileSize{fileSize} {}

FileStreamProxy::~FileStreamProxy() {
    m_Offset = 0;
    m_FileSize = 0;
    m_pFileStream = nullptr;
}

FndResult FileStreamProxy::Open(const char* filePath, AccessMode openMode) {
    return m_pFileStream->Open(filePath, openMode);
}

void FileStreamProxy::Close() {
    m_pFileStream->Close();
}

void FileStreamProxy::Flush() {
    m_pFileStream->Flush();
}

bool FileStreamProxy::IsOpened() const {
    return m_pFileStream->IsOpened();
}

bool FileStreamProxy::CanRead() const {
    return m_pFileStream->CanRead();
}

bool FileStreamProxy::CanWrite() const {
    return m_pFileStream->CanWrite();
}

bool FileStreamProxy::CanSeek() const {
    return m_pFileStream->CanSeek();
}
} // namespace nn::atk::detail::fnd