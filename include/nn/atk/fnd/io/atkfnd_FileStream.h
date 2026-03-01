#pragma once

#include <nn/atk/fnd/io/atkfnd_Stream.h>
#include <nn/atk/fnd/io/atkfnd_StreamCache.h>

namespace nn::atk::detail::fnd {
class FileStream : Stream {
public:
    enum AccessMode {
        AccessMode_None,
        AccessMode_Read,
        AccessMode_Write,
        AccessMode_ReadAndWrite,
        AccessMode_AllowAppend,
        AccessMode_AllowAppendAndWrite = 6,
    };

    virtual FndResult Open(const char* filePath, AccessMode openMode);
    virtual void Flush();
    
    virtual void EnableCache(void* buffer, size_t length);
    virtual void DisableCache();
    virtual bool IsCacheEnabled() const;

    virtual s32 GetIoBufferAlignment() const;

    virtual bool CanSetFsAccessLog() const;
    virtual void* SetFsAccessLog(FsAccessLog* pFsAccessLog);

    virtual position_t GetCachePosition();
    virtual size_t GetCachedLength();

    ~FileStream() override;
};
static_assert(sizeof(FileStream) == 0x8);
} // namespace nn::atk::detail::fnd