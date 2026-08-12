#pragma once

#include <nn/atk/atk_StreamSoundFile.h>

namespace nn::atk::detail {
class StreamSoundFileReader {
public:
    StreamSoundFileReader();

    void Initialize(const void* streamSoundFile);

    static bool IsValidFileHeader(const void* streamSoundFile);

    void Finalize();

    bool IsTrackInfoAvailable() const;
    bool IsOriginalLoopAvailable() const;

    static bool IsOriginalLoopAvailableImpl(const StreamSoundFile::FileHeader* header);

    bool IsCrc32CheckAvailable() const;
    bool IsRegionIndexCheckAvailable() const;

    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const;
    bool ReadStreamTrackInfo(StreamSoundFile::TrackInfo* pTrackInfo, s32 trackIndex) const;
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                 s32 channelIndex) const;

    u32 GetSeekBlockOffset() const {
        if (m_pHeader != nullptr && m_pHeader->HasSeekBlock())
            return m_pHeader->GetSeekBlockOffset();
        
        return 0;
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

private:
    StreamSoundFile::FileHeader* m_pHeader;
    StreamSoundFile::InfoBlockBody* m_pInfoBlockBody;
};
static_assert(sizeof(StreamSoundFileReader) == 0x10);
} // namespace nn::atk::detail