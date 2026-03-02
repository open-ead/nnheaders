#pragma once

#include <nn/atk/detail/atk_IRegionInfoReadable.h>
#include <nn/atk/detail/strm/atk_StreamSoundPrefetchFile.h>

namespace nn::atk::detail {
class StreamSoundPrefetchFileReader : IRegionInfoReadable {
public:
    struct PrefetchDataInfo {
        u32 startFrame;
        u32 prefetchSize;
        void* dataAddress;
    };

    StreamSoundPrefetchFileReader();
    ~StreamSoundPrefetchFileReader() override;

    void Initialize(const void* streamSoundPrefetchFile);
    void Finalize();

    bool IsValidFileHeader(const void* streamSoundPrefetchFile) const;

    s32 GetRegionDataOffset() const;
    u16 GetRegionInfoBytes() const;

    bool IsIncludeRegionInfo() const;
    bool IsCrc32CheckAvailable() const;
    bool IsRegionIndexCheckAvailable() const;
    
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                 s32 channelIndex) const;
    bool ReadPrefetchDataInfo(PrefetchDataInfo* pDataInfo, s32 prefetchIndex) const;
    bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, 
                        u32 regionIndex) const override;

private:
    StreamSoundPrefetchFile::FileHeader* m_pHeader;
    StreamSoundFile::InfoBlockBody* m_pInfoBlockBody;
    StreamSoundPrefetchFile::PrefetchDataBlockBody* m_pPrefetchDataBlockBody;
    u32 m_RegionDataOffset;
    u16 m_RegionInfoBytes;
};
static_assert(sizeof(StreamSoundPrefetchFileReader) == 0x28);
} // namespace nn::atk::detail