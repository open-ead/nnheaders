#pragma once

#include <nn/atk/detail/strm/atk_StreamSoundFile.h>

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

private:
    StreamSoundFile::FileHeader* m_pHeader;
    StreamSoundFile::InfoBlockBody* m_pInfoBlockBody;
};
static_assert(sizeof(StreamSoundFileReader) == 0x10);
} // namespace nn::atk::detail