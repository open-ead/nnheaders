#pragma once

#include <nn/atk/detail/wsd/atk_WaveSoundFile.h>

namespace nn::atk::detail {
struct WaveSoundInfo {
    f32 pitch;
    AdshrCurve adshr;
    u8 pan;
    u8 surroundPan;
    u8 mainSend;
    u8 fxSend[3];
    u8 lpfFreq;
    u8 biquadType;
    u8 biquadValue;
};
static_assert(sizeof(WaveSoundInfo) == 0x14);

struct WaveSoundNoteInfo {
    u32 waveArchiveId;
    s32 waveIndex;
    AdshrCurve adshr;
    u8 originalKey;
    u8 pan;
    u8 surroundPan;
    u8 volume;
    f32 pitch;
};
static_assert(sizeof(WaveSoundNoteInfo) == 0x18);

class WaveSoundFileReader {
public:
    explicit WaveSoundFileReader(const void* waveSoundFile);

    s32 GetWaveSoundCount() const;
    s32 GetNoteInfoCount(u32) const;
    s32 GetTrackInfoCount(u32) const;

    bool ReadWaveSoundInfo(WaveSoundInfo* dst, u32 index) const;

    bool IsFilterSupportedVersion() const;

    bool ReadNoteInfo(WaveSoundNoteInfo* dst, u32 index, u32 noteIndex) const;

private:
    WaveSoundFile::FileHeader* m_pHeader;
    WaveSoundFile::InfoBlockBody* m_pInfoBlockBody;
};
static_assert(sizeof(WaveSoundFileReader) == 0x10);
} // namespace nn::atk::detail