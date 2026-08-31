#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundArchiveFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn::atk {
class FsSoundArchive : public SoundArchive {
public:
    static const int BufferAlignSize {64};

    struct OpenArg {

    };

    FsSoundArchive();
    ~FsSoundArchive() override;

    bool Open(const char* path);
    bool Open(const char* path, const OpenArg* arg);
    void Close();

    size_t GetHeaderSize() const;

    bool LoadHeader(void* buffer, size_t size);

    size_t GetLabelStringDataSize() const;

    bool LoadLabelStringData(void* buffer, size_t size);

    size_t detail_GetRequiredStreamBufferSize() const override;
    const void* detail_GetFileAddress(ItemId itemId) const override;

    enum FileAccessMode {
        FileAccessMode_Always,
        FileAccessMode_InFunction
    };

    void SetFileAccessMode(FileAccessMode value);
    FileAccessMode GetFileAccessMode() const;

    void FileAccessBegin() const override;
    void FileAccessEnd() const override;

protected:
    detail::fnd::FileStream* OpenStream(void* buffer, size_t size, 
                                        position_t begin, size_t length) const override;

    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t size, const char* extFilePath,
                                           void* cacheBuffer, size_t cacheSize) const override;

    bool LoadFileHeader();

private:
    detail::SoundArchiveFileReader m_ArchiveReader;
    detail::fnd::FileStreamImpl m_FileStream;
    bool m_IsOpened {false};
    u8 m_FileAccessMode {FileAccessMode_Always};
    u8 m_Padding[2];
    u32 m_FileAccessCount {0};
    char m_SoundArchiveFullPath[SoundArchive::FilePathMax];
    detail::fnd::CriticalSection m_FileOpenCloseLock;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(FsSoundArchive) == 0x618);
#else
static_assert(sizeof(FsSoundArchive) == 0x610);
#endif
} // namespace nn::atk