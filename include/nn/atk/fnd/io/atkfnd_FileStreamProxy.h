#pragma once

#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail::fnd {
class FileStreamProxy : FileStream {
public:
    FileStreamProxy(const FileStream& fileStream, 
                    position_t offset, size_t fileSize);
    
    ~FileStreamProxy() override;

    FndResult Open(const char* filePath, AccessMode openMode) override;
    void Close() override;
    void Flush() override;
    
    bool IsOpened() const override;

    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    size_t GetSize() const override;

    size_t Read(void* buffer, size_t length, FndResult* result) override;
    size_t Write(const void* buffer, size_t length, FndResult* result) override;
    FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) override;

    position_t GetCurrentPosition() const override;

    void EnableCache(void* buffer, size_t length) override;
    void DisableCache() override;
    bool IsCacheEnabled() const override;

    s32 GetIoBufferAlignment() const override;

    bool CanSetFsAccessLog() const override;

    void* SetFsAccessLog(FsAccessLog* pFsAccessLog) override;

    position_t GetCachePosition() override;

    size_t GetCachedLength() override;

private:
    FileStream* m_pFileStream;
    position_t m_Offset;
    size_t m_FileSize;
};
static_assert(sizeof(FileStreamProxy) == 0x20);
} // namespace nn::atk::detail::fnd