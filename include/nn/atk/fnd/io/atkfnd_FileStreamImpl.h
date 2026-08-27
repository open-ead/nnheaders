#pragma once

#include <nn/fs/fs_types.h>

#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail::fnd {
class FileStreamImpl : public FileStream {
    NN_NO_COPY(FileStreamImpl);
public:
    class DirectStream : public Stream {
    public:
        DirectStream() = default;

        void Initialize(FileStreamImpl& fileStream);
        
        void Close() override {}

        bool IsOpened() const override {
            return m_Owner->IsOpened();
        }
        
        bool CanRead() const override {
            return m_Owner->CanRead();
        }

        bool CanWrite() const override {
            return m_Owner->CanWrite();
        }

        bool CanSeek() const override {
            return m_Owner->CanSeek();
        }

        size_t GetSize() const override {
            return m_Owner->GetSize();
        }

        size_t Read(void* buf, size_t length, FndResult* result) override {
            return m_Owner->ReadDirect(buf, length, result);
        }

        size_t Write(const void* buf, size_t length, FndResult* result) override {
            return m_Owner->WriteDirect(buf, length, result);
        }

        FndResult Seek(position_t offset, SeekOrigin origin) override {
            return m_Owner->SeekDirect(offset, origin);
        }
        
        position_t GetCurrentPosition() const override {
            return m_Owner->GetCurrentPosition();
        }

        ~DirectStream() override;

    private:
        FileStreamImpl* m_Owner;
    };
    static_assert(sizeof(DirectStream) == 0x10);

    FileStreamImpl();
    explicit FileStreamImpl(void*);
    ~FileStreamImpl() override;

    FndResult Open(const char* filePath, AccessMode accessMode) override;
    void Close() override;
    void Flush() override;

    bool IsOpened() const override;

    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    size_t GetSize() const override;

    size_t Read(void* buf, size_t length, FndResult* result) override;
    size_t Write(const void* buf, size_t length, FndResult* result) override;
    FndResult Seek(position_t offset, SeekOrigin origin) override;
    
    position_t GetCurrentPosition() const override {
        return m_CurrentPosition;
    }

    void EnableCache(void* buffer, size_t length) override;
    void DisableCache() override;
    bool IsCacheEnabled() const override;

    int GetIoBufferAlignment() const override;

    bool CanSetFsAccessLog() const override;
    void* SetFsAccessLog(FsAccessLog* pFsAccessLog) override;

    position_t GetCachePosition() override;
    size_t GetCachedLength() override;

private:
    size_t ReadDirect(void* buf, size_t length, FndResult* result);
    size_t WriteDirect(const void* buf, size_t length, FndResult* result);
    FndResult SeekDirect(position_t offset, SeekOrigin origin);

    void ValidateAlignment([[maybe_unused]] const void* buf) const;

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