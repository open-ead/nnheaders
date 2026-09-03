#include <nn/atk/atk_FsSoundArchive.h>

#include <new>

#include <nn/atk/fnd/os/atkfnd_ScopedLock.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamProxy.h>

namespace nn::atk {
FsSoundArchive::FsSoundArchive() = default;

FsSoundArchive::~FsSoundArchive() {
    Close();
}

void FsSoundArchive::Close() {
    FileAccessBegin();

    if (m_IsOpened) {
        m_FileStream.Close();
        m_IsOpened = false;
    }

    Finalize();
}

bool FsSoundArchive::LoadHeader(void* buffer, size_t size) {
    const s32 infoChunkOffset {m_ArchiveReader.GetInfoBlockOffset()};
    const u32 infoChunkSize {m_ArchiveReader.GetInfoBlockSize()};

    if (size >= infoChunkSize) {
        FileAccessBegin();
        m_FileStream.Seek(infoChunkOffset, detail::fnd::Stream::SeekOrigin_Begin);
        size_t readSize {m_FileStream.Read(buffer, infoChunkSize, nullptr)};

        if (readSize == infoChunkSize) {
            FileAccessEnd();
            m_ArchiveReader.SetInfoBlock(buffer);
            return true;
        }
    }

    return false;
}

bool FsSoundArchive::LoadLabelStringData(void* buffer, size_t size) {
    const s32 stringBlockOffset {m_ArchiveReader.GetStringBlockOffset()};
    const u32 stringBlockSize {m_ArchiveReader.GetStringBlockSize()};

    if (stringBlockOffset != detail::Util::Reference::InvalidOffset && size >= stringBlockSize) {
        FileAccessBegin();
        m_FileStream.Seek(stringBlockOffset, detail::fnd::Stream::SeekOrigin_Begin);
        size_t readSize {m_FileStream.Read(buffer, stringBlockSize, nullptr)};

        if (readSize == stringBlockSize) {
            FileAccessEnd();
            m_ArchiveReader.SetStringBlock(buffer);
            return true;
        }
    }

    return false;
}

size_t FsSoundArchive::detail_GetRequiredStreamBufferSize() const {
    return 8;
}

const void* FsSoundArchive::detail_GetFileAddress([[maybe_unused]] ItemId itemId) const {
    return nullptr;
} 

void FsSoundArchive::FileAccessBegin() const {
    if (m_FileAccessMode == FileAccessMode_InFunction) {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock{m_FileOpenCloseLock};
        if (m_FileAccessCount == 0)
            m_FileStream.Open(m_SoundArchiveFullPath, detail::fnd::FileStream::AccessMode_Read);
    
        ++m_FileAccessCount;
    }
}

void FsSoundArchive::FileAccessEnd() const {
    if (m_FileAccessMode == FileAccessMode_InFunction) {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock{m_FileOpenCloseLock};
        if (m_FileAccessCount == 1)
            m_FileStream.Close();
        
        if (m_FileAccessCount != 0)
            --m_FileAccessCount;
    }
}

// NON_MATCHING
detail::fnd::FileStream* FsSoundArchive::OpenStream(void* buffer, size_t size, 
                                                    position_t begin, size_t length) const {
    detail::fnd::FileStream* stream {};
    if (sizeof(detail::fnd::FileStreamProxy) <= size && m_IsOpened) {
        stream = new (buffer) detail::fnd::FileStreamProxy(&m_FileStream, begin, length);
    }

    return stream;
}

// NON_MATCHING
detail::fnd::FileStream* FsSoundArchive::OpenExtStream(void* buffer, size_t size, const char* extFilePath,
                                                       void* cacheBuffer, size_t cacheSize) const {
    detail::fnd::FileStream* fileStream {};
    if (size >= sizeof(detail::fnd::FileStreamImpl) && m_IsOpened) {
        fileStream = new (buffer) detail::fnd::FileStreamImpl();

        fileStream->Open(extFilePath, detail::fnd::FileStreamImpl::AccessMode_Read);

        if (!fileStream->IsOpened()) {
            fileStream = nullptr;
        }
        else {
            if (cacheBuffer != nullptr && cacheSize != 0)
                fileStream->EnableCache(cacheBuffer, cacheSize);
        }
    }

    return fileStream;
}

bool FsSoundArchive::LoadFileHeader() {
    const int Align = 256;
    const size_t headerAlignSize = 256;

    char headerArea[sizeof(detail::SoundArchiveFile::FileHeader) + headerAlignSize + Align];
    void* file {util::BytePtr(headerArea).AlignUp(Align).Get()};
    
    size_t readSize {m_FileStream.Read(file, headerAlignSize, nullptr)};

    if (readSize != headerAlignSize) 
        return false;
    
    m_ArchiveReader.Initialize(file);
    Initialize(&m_ArchiveReader);
    return true;
}
} // namespace nn::atk

