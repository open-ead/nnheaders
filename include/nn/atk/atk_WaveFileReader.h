#pragma once

#include <nn/atk/atk_WaveFile.h>
#include <nn/atk/atk_DspadpcmReader.h>

namespace nn::atk::detail {
class WaveFileReader {
public:
    static const int SignatureFile = 0x56415746; // FWAV

    static SampleFormat GetSampleFormat(u8 format);

    WaveFileReader(const void* waveFile, s8 waveType);

    bool IsAvailable() const { return m_pHeader != nullptr; }

    bool IsOriginalLoopAvailable() const;

    bool ReadWaveInfo(WaveInfo* info, const void* waveDataOffsetOrigin) const;

    const void* GetWaveDataAddress(const WaveFile::ChannelInfo* info, const void* waveDataOffsetOrigin) const;

private:
    const WaveFile::FileHeader* m_pHeader;
    const WaveFile::InfoBlockBody* m_pInfoBlockBody;
    const void* m_pDataBlockBody;
    DspadpcmReader m_DspadpcmReader;
    s8 m_WaveType;
};
static_assert(sizeof(WaveFileReader) == 0x28);
} // namespace nn::atk::detail