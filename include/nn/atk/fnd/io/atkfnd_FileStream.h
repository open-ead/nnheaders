#pragma once

#include <nn/atk/fnd/io/atkfnd_Stream.h>
#include <nn/atk/fnd/io/atkfnd_StreamCache.h>

namespace nn::atk::detail::fnd {
class FileStream : public Stream {
public:
    enum AccessMode {
        AccessMode_None                 = 0,
        AccessMode_Read                 = 1,
        AccessMode_Write                = 2,
        AccessMode_AllowAppend          = 4,
        AccessMode_ReadAndWrite         = AccessMode_Read | AccessMode_Write,
        AccessMode_AllowAppendAndWrite  = AccessMode_AllowAppend | AccessMode_Write,
    };

    ~FileStream() override = default;

    virtual FndResult Open(const char* filePath, AccessMode openMode) = 0;
    void Close() override = 0;
    virtual void Flush() = 0;
    
    bool IsOpened() const override = 0;

    bool CanRead() const override = 0;
    bool CanWrite() const override = 0;
    bool CanSeek() const override = 0;

    size_t GetSize() const override = 0;

    size_t Read(void* buf, size_t length, FndResult* result) override = 0;
    size_t Write(const void* buf, size_t length, FndResult* result) override = 0;
    FndResult Seek(position_t offset, SeekOrigin origin) override = 0;

    position_t GetCurrentPosition() const override = 0;

    virtual void EnableCache(void* buffer, size_t length) = 0;
    virtual void DisableCache() = 0;
    virtual bool IsCacheEnabled() const = 0;

    virtual int GetIoBufferAlignment() const = 0;

    virtual bool CanSetFsAccessLog() const = 0;
    virtual void* SetFsAccessLog(FsAccessLog* pFsAccessLog) = 0;

    virtual position_t GetCachePosition() = 0;
    virtual size_t GetCachedLength() = 0;
};
static_assert(sizeof(FileStream) == 0x8);
} // namespace nn::atk::detail::fnd