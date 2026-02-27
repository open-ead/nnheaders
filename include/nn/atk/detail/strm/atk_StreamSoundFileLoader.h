#pragma once

#include <nn/atk/detail/atk_IRegionInfoReadable.h>
#include <nn/atk/detail/strm/atk_StreamSoundFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {
class StreamSoundFileLoader : IRegionInfoReadable {
public:
    StreamSoundFileLoader();
    ~StreamSoundFileLoader() override;

    bool LoadFileHeader(StreamSoundFileReader* reader, void* buffer, u64 size);

    bool ReadSeekBlockData(u16* yn1, u16 yn2, s32 blockIndex, s32 channelCount);
    bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 regionIndex) const override;

private:
    fnd::FileStream* m_pStream;
    u32 m_SeekBlockOffset;
    u32 m_RegionDataOffset;
    u16 m_RegionInfoBytes;
};
static_assert(sizeof(StreamSoundFileLoader) == 0x20);
} // namespace nn::atk::detail