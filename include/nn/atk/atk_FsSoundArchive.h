#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_SoundArchiveFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn::atk {
class FsSoundArchive : SoundArchive {
public:
    enum FileAccessMode {
        FileAccessMode_Always,
        FileAccessMode_InFunction
    };

    FsSoundArchive();
    ~FsSoundArchive() override;

    void Close();
    bool Open(const char* path);

    detail::fnd::FileStream* OpenStream(void* buffer, std::size_t size, 
                                        position_t begin, std::size_t length) const override;

    detail::fnd::FileStream* OpenExtStream(void* buffer, std::size_t size, const char* extFilePath,
                                           void* cacheBuffer, std::size_t cacheSize) const;

    std::size_t detail_GetRequiredStreamBufferSize() const override;

    bool LoadHeader(void* buffer, std::size_t size);
    bool LoadLabelStringData(void* buffer, std::size_t size);

    void FileAccessBegin() const override;
    void FileAccessEnd() const override;

    void* detail_GetFileAddress(ItemId itemId) override;

private:
    detail::SoundArchiveFileReader m_ArchiveReader;
    detail::fnd::FileStreamImpl m_FileStream;
    bool m_IsOpened;
    u8 m_FileAccessMode;
    u8 m_Padding[2];
    u32 m_FileAccessCount;
    char m_SoundArchiveFullPath[639];
    detail::fnd::CriticalSection m_FileOpenCloseLock;
};
static_assert(sizeof(FsSoundArchive) == 0x610);
} // namespace nn::atk