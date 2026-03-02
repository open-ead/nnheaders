#pragma once

#include <nn/fs/fs_types.h>

#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail::fnd {
class FileStreamImpl : FileStream {
public:
    class DirectStream : fnd::Stream {
    public:
        ~DirectStream() override;

        void Close() override;
        bool IsOpened() const override;
        
        size_t Read(void* buf, size_t length, FndResult* result) override;
        size_t Write(const void* buf, size_t length, FndResult* result) override;
        FndResult Seek(position_t offset, SeekOrigin origin) override;
        
        position_t GetCurrentPosition() const override;
        size_t GetSize() const override;
        
        bool CanRead() const override;
        bool CanWrite() const override;
        bool CanSeek() const override;
    
    private:
        FileStreamImpl* m_Owner;
    };
    static_assert(sizeof(DirectStream) == 0x10);

    FileStreamImpl();
    ~FileStreamImpl() override;

    size_t Read(void* buf, size_t length, FndResult* result) override;
    size_t Write(const void* buf, size_t length, FndResult* result) override;
    FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) override;
    
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    void EnableCache(void* buffer, size_t length) override;
    void DisableCache() override;

    void ValidateAlignment(void* buf);

    FndResult Open(const char* filePath, AccessMode accessMode) override;
    void Close() override;
    void Flush() override;

    bool IsOpened() const override;

    size_t GetSize() const override;
    s32 GetIoBufferAlignment() const override;

    size_t ReadDirect(void* buf, size_t length, FndResult* result);
    size_t WriteDirect(const void* buf, size_t length, FndResult* result);
    FndResult SeekDirect(position_t offset, fnd::Stream::SeekOrigin origin);
    
    position_t GetCurrentPosition() const override;
    
    bool IsCacheEnabled() const override;

    bool CanSetFsAccessLog() const override;
    void* SetFsAccessLog(FsAccessLog* pFsAccessLog) override;

    position_t GetCachePosition() override;
    size_t GetCachedLength() override;

private:
    fs::FileHandle m_FileHandle;
    bool m_IsOpened;
    u8 m_Padding[3];
    size_t m_FileSize;
    position_t m_CurrentPosition;
    StreamCache m_StreamCache;
    DirectStream m_DirectStream;
    FsAccessLog* m_pAccessLog;
};
static_assert(sizeof(FileStreamImpl) == 0x80);
} // namespace nn::atk::detail::fnd