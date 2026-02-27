#pragma once

#include <nn/atk/detail/atk_WaveFile.h>
#include <nn/atk/detail/dsp/atk_DspadpcmReader.h>

namespace nn::atk::detail {
class WaveFileReader {
public:
    constexpr static s32 SignatureFile = 0x56415746; // FWAV

    static SampleFormat GetSampleFormat(u8 format);

    WaveFileReader(void* waveFile, s8 waveType);

    bool IsOriginalLoopAvailable() const;

    bool ReadWaveInfo(WaveInfo* info, const void* waveDataOffsetOrigin) const;

    void* GetWaveDataAddress(const WaveFile::ChannelInfo* channelInfo, const void*) const;

private:
    WaveFile::FileHeader* m_pHeader;
    WaveFile::InfoBlockBody* m_pInfoBlockBody;
    void* m_pDataBlockBody;
    DspadpcmReader m_DspadpcmReader;
    s8 m_WaveType;
};
static_assert(sizeof(WaveFileReader) == 0x28);
} // namespace nn::atk::detail