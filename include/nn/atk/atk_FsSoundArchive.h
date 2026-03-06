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

    constexpr static u32 BufferAlignSize = 64;

    FsSoundArchive();
    ~FsSoundArchive() override;

    void Close();
    bool Open(const char* path);

    bool LoadFileHeader();

    detail::fnd::FileStream* OpenStream(void* buffer, size_t size, 
                                        position_t begin, size_t length) const override;

    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t size, const char* extFilePath,
                                           void* cacheBuffer, size_t cacheSize) const override;

    size_t detail_GetRequiredStreamBufferSize() const override;

    bool LoadHeader(void* buffer, size_t size);
    bool LoadLabelStringData(void* buffer, size_t size);

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
    char m_SoundArchiveFullPath[SoundArchive::FilePathMax];
    detail::fnd::CriticalSection m_FileOpenCloseLock;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(FsSoundArchive) == 0x618);
#else
static_assert(sizeof(FsSoundArchive) == 0x610);
#endif
} // namespace nn::atk