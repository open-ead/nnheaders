#pragma once

#include <nn/atk/atk_WaveSoundFile.h>

namespace nn::atk::detail {
struct WaveSoundInfo {
    float pitch;
    AdshrCurve adshr;
    u8 pan;
    u8 surroundPan;
    u8 mainSend;
    u8 fxSend[AuxBus_Count];
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
    float pitch;

    WaveSoundNoteInfo()
        : adshr(0, 0, 0, 0, 0) {};
};
static_assert(sizeof(WaveSoundNoteInfo) == 0x18);

class WaveSoundFileReader {
public:
    static const u32 SignatureFile{0x44535746}; // FWSD

    explicit WaveSoundFileReader(const void* waveSoundFile);

    bool IsAvailable() const { return m_pHeader != nullptr; }

    u32 GetWaveSoundCount() const;
    u32 GetNoteInfoCount(u32 index) const;
    u32 GetTrackInfoCount(u32 index) const;

    bool ReadWaveSoundInfo(WaveSoundInfo* dst, u32 index) const;

    bool ReadNoteInfo(WaveSoundNoteInfo* dst, u32 index, u32 noteIndex) const;
    
    bool IsFilterSupportedVersion() const;

private:
    const WaveSoundFile::FileHeader* m_pHeader{};
    const WaveSoundFile::InfoBlockBody* m_pInfoBlockBody{};
};
static_assert(sizeof(WaveSoundFileReader) == 0x10);
} // namespace nn::atk::detail