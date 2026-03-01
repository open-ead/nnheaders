#pragma once

#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {
class MemoryFileStream : fnd::FileStream {
public:
    MemoryFileStream(void* buffer, size_t size);
    ~MemoryFileStream() override;

    void Close() override;

    size_t Read(void* buf, size_t length, fnd::FndResult* result) override;
    fnd::FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) override;

    bool IsOpened() const override;

    size_t Write(const void* buf, size_t length, fnd::FndResult* result) override;

    position_t GetCurrentPosition() const override;

    size_t GetSize() const override;

    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    fnd::FndResult Open(const char* filePath, AccessMode openMode) override;
    void Flush() override;

    void EnableCache(void* buffer, size_t length) override;
    void DisableCache() override;
    bool IsCacheEnabled() const override;

    s32 GetIoBufferAlignment() const override;

    bool CanSetFsAccessLog() const override;
    void* SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog) override;
    
    position_t GetCachePosition() override;
    size_t GetCachedLength() override;

private:
    void* m_pBuffer;
    size_t m_Size;
    position_t m_Position;
};
static_assert(sizeof(MemoryFileStream) == 0x20);
} // namespace nn::atk::detail