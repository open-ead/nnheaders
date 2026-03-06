#pragma once

#include <nn/util/util_BinaryFormat.h>

#include <nn/atk/detail/util/atk_BinaryTypes.h>

namespace nn::atk::detail {
struct AdvancedWaveSoundFile {    
    struct WaveSoundTrack;
    struct InfoBlockBody {

        BinaryTypes::ReferenceTable* GetTrackReferenceTable() const;
        WaveSoundTrack* GetWaveSoundTrack(s32 index) const;

        u32 offsetToTrackTableReference;
        BinaryTypes::Reference toTrackTable;
    };
    static_assert(sizeof(InfoBlockBody) == 0x8);

    struct InfoBlock {
        util::BinaryBlockHeader blockHeader;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x18);

    struct WaveSoundClip;
    struct WaveSoundTrack {

        BinaryTypes::ReferenceTable* GetClipReferenceTable() const;
        WaveSoundClip* GetWaveSoundClip(s32 index) const;

        u32 offsetToCurveTableReference;
        u32 offsetToClipTableReference;
        BinaryTypes::Reference toClipTable;
    };

    struct WaveSoundClip {
        u32 waveIndex;
        u32 position;
        u32 duration;
        u32 startOffset;
        float pitch;
        u8 volume;
        u8 pan;
        u8 padding[2];
    };
    static_assert(sizeof(WaveSoundClip) == 0x18);

    InfoBlock* GetBlock() const;
    
    util::BinaryFileHeader fileHeader;
};
static_assert(sizeof(AdvancedWaveSoundFile) == 0x20);

} // namespace nn::atk::detail