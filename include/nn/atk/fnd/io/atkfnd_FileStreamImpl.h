#pragma once

#include <nn/fs/fs_types.h>

#include <nn/atk/fnd/io/atkfnd_FileStream.h>
#include <nn/atk/fnd/io/atkfnd_StreamCache.h>

namespace nn::atk::detail::fnd {
class FileStreamImpl : FileStream {
public:
    class DirectStream : fnd::Stream {
    public:
        ~DirectStream() override;

        void Close() override;
        bool IsOpened() const override;
        
        std::size_t Read(void* buf, std::size_t length, FndResult* result) override;
        std::size_t Write(void* buf, std::size_t length, FndResult* result) override;
        FndResult Seek(position_t offset, SeekOrigin origin) override;
        
        position_t GetCurrentPosition() const override;
        std::size_t GetSize() const override;
        
        bool CanRead() const override;
        bool CanWrite() const override;
        bool CanSeek() const override;
    
    private:
        FileStreamImpl* m_Owner;
    };
    static_assert(sizeof(DirectStream) == 0x10);

    FileStreamImpl();
    ~FileStreamImpl() override;

    std::size_t Read(void* buf, std::size_t length, FndResult* result) override;
    std::size_t Write(void* buf, std::size_t length, FndResult* result) override;
    FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) override;
    
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    void ValidateAlignment(void* buf);

    FndResult Open(char* filePath, AccessMode accessMode);
    void Close() override;

    bool IsOpened() const override;

    std::size_t GetSize() const override;
    position_t GetCurrentPosition() const override;

    std::size_t ReadDirect(void* buf, std::size_t length, FndResult* result);
    std::size_t WriteDirect(void* buf, std::size_t length, FndResult* result);
    FndResult SeekDirect(position_t offset, fnd::Stream::SeekOrigin origin);

    virtual void Flush();
    
    virtual void EnableCache(void* buffer, std::size_t length);
    virtual void DisableCache();
    virtual bool IsCacheEnabled();

    virtual s32 GetIoBufferAlignment();

    virtual bool CanSetFsAccessLog();
    virtual void* SetFsAccessLog(FsAccessLog* pFsAccessLog);

    virtual position_t GetCachePosition();
    virtual std::size_t GetCachedLength();

private:
    fs::FileHandle m_FileHandle;
    bool m_IsOpened;
    u8 m_Padding[3];
    std::size_t m_FileSize;
    position_t m_CurrentPosition;
    StreamCache m_StreamCache;
    DirectStream m_DirectStream;
    FsAccessLog* m_pAccessLog;
};
static_assert(sizeof(FileStreamImpl) == 0x80);
} // namespace nn::atk::detail::fnd