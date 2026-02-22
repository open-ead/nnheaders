#pragma once

#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct WaveSoundFile {
    struct InfoBlock;
    struct FileHeader : Util::SoundFileHeader {
        InfoBlock* GetInfoBlock();
    };

    struct WaveSoundData;
    struct InfoBlockBody {
        Util::Reference toWaveIdTable;
        Util::Reference toWaveSoundDataReferenceTable;

        WaveSoundData* GetWaveSoundData(u32 index);
        Util::WaveIdTable* GetWaveIdTable();
    };
    static_assert(sizeof(InfoBlockBody) == 0x10);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x18);

    struct NoteInfo;
    struct WaveSoundInfo;
    struct WaveSoundData {
        Util::Reference toWaveSoundInfo;
        Util::Reference toTrackInfoReferenceTable;
        Util::Reference toNoteInfoReferenceTable;

        WaveSoundInfo* GetWaveSoundInfo();
        NoteInfo* GetNoteInfo(u32 index);
    };
    static_assert(sizeof(WaveSoundData) == 0x18);

    struct WaveSoundInfo {
        Util::BitFlag optionParameter;

        u8 GetPan();
        s8 GetSurroundPan();
        f32 GetPitch();
        void GetSendValue(u8* mainSend, u8* fxSend, u8 fxSendCount);
        AdshrCurve* GetAdshrCurve();
        u8 GetLpfFreq();
        u8 GetBiquadType();
        u8 GetBiquadValue();
    };
    static_assert(sizeof(WaveSoundInfo) == 0x4);

    struct NoteInfo {
        u32 waveIdTableIndex;
        Util::BitFlag optionParameter;

        u8 GetOriginalKey();
        u8 GetVolume();
        u8 GetPan();
        u8 GetSurroundPan();
        f32 GetPitch();
        AdshrCurve* GetAdshrCurve();
    };
    static_assert(sizeof(NoteInfo) == 0x8);

    struct TrackInfo {
        Util::Reference toNoteEventReferenceTable;
    };
    static_assert(sizeof(TrackInfo) == 0x8);
};
} // namespace nn::atk::detail