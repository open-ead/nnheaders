#pragma once

#include <nn/atk/detail/atk_AdvancedWaveSoundFile.h>

namespace nn::atk::detail {
struct AdvancedWaveSoundClipInfo {
    u32 waveIndex;
    u32 position;
    u32 duration;
    u32 startOffset;
    f32 pitch;
    u8 volume;
    u8 pan;
};
static_assert(sizeof(AdvancedWaveSoundClipInfo) == 0x18);

struct AdvancedWaveSoundTrackInfo {
    constexpr static u32 AdvancedWaveSoundClipInfoCountMax = 10;

    s32 waveSoundClipCount;
    AdvancedWaveSoundClipInfo waveSoundClipInfo[AdvancedWaveSoundClipInfoCountMax];
};
static_assert(sizeof(AdvancedWaveSoundTrackInfo) == 0xf4);

struct AdvancedWaveSoundTrackInfoSet {
    constexpr static u32 AdvancedWaveSoundTrackInfoCountMax = 4;

    s32 waveSoundTrackCount;
    AdvancedWaveSoundTrackInfo waveSoundTrackInfo[AdvancedWaveSoundTrackInfoCountMax];
};
static_assert(sizeof(AdvancedWaveSoundTrackInfoSet) == 0x3d4);

class AdvancedWaveSoundFileReader {
public:
    explicit AdvancedWaveSoundFileReader(const void* pFile);

    s32 GetWaveSoundTrackCount();
    s32 GetWaveSoundClipCount(s32 index);

    bool ReadWaveSoundTrackInfoSet(AdvancedWaveSoundTrackInfoSet* pTrackInfoSet);

private:
    AdvancedWaveSoundFile::InfoBlockBody* m_pInfoBlockBody;
};
static_assert(sizeof(AdvancedWaveSoundFileReader) == 0x8);
} // namespace nn::atk::detail