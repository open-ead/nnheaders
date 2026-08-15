#pragma once

#include <nn/atk/detail/atk_IRegionInfoReadable.h>
#include <nn/atk/atk_StreamSoundPrefetchFile.h>

namespace nn::atk::detail {
class StreamSoundPrefetchFileReader : public IRegionInfoReadable {
public:
    struct PrefetchDataInfo {
        u32 startFrame;
        u32 prefetchSize;
        const void* dataAddress;
    };

    StreamSoundPrefetchFileReader();
    ~StreamSoundPrefetchFileReader() override;

    void Initialize(const void* streamSoundPrefetchFile);
    void Finalize() {
        m_pHeader = nullptr;
        m_pInfoBlockBody = nullptr;
        m_pPrefetchDataBlockBody = nullptr;
        m_RegionDataOffset = 0;
        m_RegionInfoBytes = 0;
    }

    bool IsAvailable() const {
        return m_pHeader != nullptr;
    }

    bool IsIncludeRegionInfo() const;
    bool IsCrc32CheckAvailable() const;
    bool IsRegionIndexCheckAvailable() const;

    bool IsValidFileHeader(const void* streamSoundPrefetchFile) const;

    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                 int channelIndex) const;
    bool ReadPrefetchDataInfo(PrefetchDataInfo* pDataInfo, int prefetchIndex) const;
    
    bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo,u32 regionIndex) const override;

    u32 GetChannelCount() const;
    u32 GetPrefetchDataCount() const;

    u32 GetRegionDataOffset() const {
        u32 result {0};

        if (IsAvailable() && m_pHeader->HasRegionBlock()) {
            result = m_pHeader->GetRegionBlockOffset() + sizeof(BinaryBlockHeader)
                     + m_pInfoBlockBody->GetStreamSoundInfo()->regionDataOffset.offset;
        }

        return result;
    }

    u16 GetRegionInfoBytes() const {
        return m_pInfoBlockBody->GetStreamSoundInfo()->regionInfoBytes;
    }

private:
    const StreamSoundPrefetchFile::FileHeader* m_pHeader {};
    const StreamSoundFile::InfoBlockBody* m_pInfoBlockBody {};
    const StreamSoundPrefetchFile::PrefetchDataBlockBody* m_pPrefetchDataBlockBody {};
    u32 m_RegionDataOffset {0};
    u16 m_RegionInfoBytes {0};
};
static_assert(sizeof(StreamSoundPrefetchFileReader) == 0x28);
} // namespace nn::atk::detail