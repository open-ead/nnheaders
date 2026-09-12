#pragma once

#include <nn/atk/detail/atk_IRegionInfoReadable.h>
#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {
class StreamSoundFileLoader : public IRegionInfoReadable {
public:
    StreamSoundFileLoader() = default;
    explicit StreamSoundFileLoader(fnd::FileStream* stream) {
        Initialize(stream);
    }
    
    void Initialize(fnd::FileStream* stream) {
        m_pStream = stream;
        m_SeekBlockOffset = 0;
        m_RegionDataOffset = 0;
        m_RegionInfoBytes = 0;
    }

    void Finalize() {
        m_pStream = nullptr;
        m_SeekBlockOffset = 0;
        m_RegionDataOffset = 0;
        m_RegionInfoBytes = 0;
    }

    bool LoadFileHeader(StreamSoundFileReader* reader, void* buffer, u64 size);

    bool ReadSeekBlockData(u16* yn1, u16* yn2, int blockIndex, int channelCount);
    bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 regionIndex) const override;
    
    ~StreamSoundFileLoader() override;

private:
    fnd::FileStream* m_pStream {};
    u32 m_SeekBlockOffset {0};
    u32 m_RegionDataOffset {0};
    u16 m_RegionInfoBytes {0};
};
static_assert(sizeof(StreamSoundFileLoader) == 0x20);
} // namespace nn::atk::detail