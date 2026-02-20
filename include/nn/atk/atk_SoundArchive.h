#pragma once

#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/detail/atk_SoundArchiveFileReader.h>

#include <nn/atk/detail/fnd/atkfnd_FileStream.h>

namespace nn::atk {
class SoundArchive {
public:
    using ItemId = u32;

    SoundArchive();

    virtual ~SoundArchive();

    virtual void* detail_GetFileAddress(ItemId itemId) = 0;
    virtual size_t detail_GetRequiredStreamBufferSize() = 0;
    
    virtual void FileAccessBegin();
    virtual void FileAccessEnd();

    virtual bool IsAddon();

    virtual detail::fnd::FileStream* OpenStream(void* buffer, size_t size, 
                                                position_t begin, size_t length) = 0;

private:
    detail::SoundArchiveFileReader* m_pFileReader{};
    detail::SoundArchiveParametersHook* m_pParametersHook{};
    char m_ExtFileRoot[639];
    u32 m_FileBlockOffset;
};
static_assert(sizeof(SoundArchive) == 0x2a0);

} // namespace nn::atk::detail