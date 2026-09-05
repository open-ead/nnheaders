#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {

class MemoryFileStream : public fnd::FileStream {
public:
    MemoryFileStream(const void* buffer, size_t size);
    ~MemoryFileStream() override = default;

    fnd::FndResult Open(const char* filePath, AccessMode openMode) override;
    void Close() override;
    void Flush() override {}

    bool IsOpened() const override { return m_pBuffer != nullptr; }

    bool CanRead() const override { return true; }
    bool CanWrite() const override { return false; }
    bool CanSeek() const override { return true; }

    size_t GetSize() const override { return m_Size; }

    size_t Read(void* buf, size_t length, fnd::FndResult* result) override;

    size_t Write([[maybe_unused]] const void* buf, [[maybe_unused]] size_t length,
                 [[maybe_unused]] fnd::FndResult* result) override {
        return 0;
    }

    fnd::FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) override;

    position_t GetCurrentPosition() const override { return m_Position; }

    void EnableCache([[maybe_unused]] void* buffer, [[maybe_unused]] size_t length) override {}
    void DisableCache() override;
    bool IsCacheEnabled() const override;

    int GetIoBufferAlignment() const override;

    bool CanSetFsAccessLog() const override;
    void* SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog) override;

    position_t GetCachePosition() override;
    size_t GetCachedLength() override;

private:
    const void* m_pBuffer;
    size_t m_Size;
    position_t m_Position;
};
static_assert(sizeof(MemoryFileStream) == 0x20);

}  // namespace nn::atk::detail