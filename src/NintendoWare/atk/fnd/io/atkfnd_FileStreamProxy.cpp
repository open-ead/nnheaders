#include <nn/atk/fnd/io/atkfnd_FileStreamProxy.h>

#include <algorithm>

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

size_t FileStreamProxy::GetSize() const {
    return m_pFileStream->GetSize();
}

size_t FileStreamProxy::Read(void* buffer, size_t length, FndResult* result) {
    return m_pFileStream->Read(buffer, length, result);
}

size_t FileStreamProxy::Write(const void* buffer, size_t length, FndResult* result) {
    return m_pFileStream->Write(buffer, length, result);
}

// NON_MATCHING: wrong csel cond and wrong use of registers
FndResult FileStreamProxy::Seek(position_t offset, fnd::Stream::SeekOrigin origin) {
    position_t fileSizeEndPosition = m_FileSize;

    switch (origin) {
    case SeekOrigin_Begin:
        offset = m_Offset + offset;
        break;
    case SeekOrigin_End:
        offset = m_Offset + (fileSizeEndPosition - offset);
        break;
    case SeekOrigin_Current:
        offset = m_pFileStream->GetCurrentPosition() + offset;
        break; 
    default:
        return FndResult{FndResultType_Failed};
    }

    offset = std::min(m_Offset + fileSizeEndPosition, offset);
    offset = std::min(m_Offset, offset);

    return m_pFileStream->Seek(offset, origin);
}

position_t FileStreamProxy::GetCurrentPosition() const {
    return m_pFileStream->GetCurrentPosition();
}

void FileStreamProxy::EnableCache(void* buffer, size_t length) {
    m_pFileStream->EnableCache(buffer, length);
}

void FileStreamProxy::DisableCache() {
    m_pFileStream->DisableCache();
}

bool FileStreamProxy::IsCacheEnabled() const {
    return m_pFileStream->IsCacheEnabled();
}
} // namespace nn::atk::detail::fnd