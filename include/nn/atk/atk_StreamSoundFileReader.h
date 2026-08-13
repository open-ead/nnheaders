#pragma once

#include <nn/atk/atk_StreamSoundFile.h>

namespace nn::atk::detail {
class StreamSoundFileReader {
public:
    StreamSoundFileReader();

    void Initialize(const void* streamSoundFile);
    void Finalize();

    bool IsAvailable() const {
        return m_pHeader != nullptr;
    }

    bool IsTrackInfoAvailable() const;
    bool IsOriginalLoopAvailable() const;
    bool IsCrc32CheckAvailable() const;
    bool IsRegionIndexCheckAvailable() const;

    static bool IsValidFileHeader(const void* streamSoundFile);

    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const;
    bool ReadStreamTrackInfo(StreamSoundFile::TrackInfo* pTrackInfo, int trackIndex) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                 int channelIndex) const;

    u32 GetChannelCount() const;
    u32 GetTrackCount() const;

    u32 GetSeekBlockOffset() const {
        if (m_pHeader != nullptr && m_pHeader->HasSeekBlock())
            return m_pHeader->GetSeekBlockOffset();
        
        return 0;
    }

    u32 GetSampleDataOffset() const {
        u32 result {0};
        if (m_pHeader != nullptr)
            result = m_pHeader->GetDataBlockOffset()
                     + m_pInfoBlockBody->GetStreamSoundInfo()->sampleDataOffset.offset
                     + sizeof(BinaryBlockHeader);

        return result;
    }

    u32 GetRegionDataOffset() const {
        u32 result {0};
        if (m_pHeader != nullptr && m_pHeader->HasRegionBlock()) {
            result = m_pHeader->GetRegionBlockOffset() 
                     + m_pInfoBlockBody->GetStreamSoundInfo()->regionDataOffset.offset 
                     + sizeof(BinaryBlockHeader);
        }
        
        return result;
    }

    u32 GetRegionInfoBytes() const {
        return m_pInfoBlockBody->GetStreamSoundInfo()->regionInfoBytes;
    }

    static bool IsOriginalLoopAvailableImpl(const StreamSoundFile::FileHeader* pHeader);

private:
    const StreamSoundFile::FileHeader* m_pHeader {};
    const StreamSoundFile::InfoBlockBody* m_pInfoBlockBody {};
};
static_assert(sizeof(StreamSoundFileReader) == 0x10);
} // namespace nn::atk::detail