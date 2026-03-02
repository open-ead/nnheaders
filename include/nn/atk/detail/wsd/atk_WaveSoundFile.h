#pragma once

#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct WaveSoundFile {
    struct InfoBlock;
    struct FileHeader : Util::SoundFileHeader {

        InfoBlock* GetInfoBlock() const;

    };

    struct WaveSoundData;
    struct InfoBlockBody {

        WaveSoundData* GetWaveSoundData(u32 index) const;
        Util::ReferenceTable* GetWaveSoundDataReferenceTable() const;

        Util::WaveIdTable* GetWaveIdTable() const;

        Util::Reference toWaveIdTable;
        Util::Reference toWaveSoundDataReferenceTable;
    };
    static_assert(sizeof(InfoBlockBody) == 0x10);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x18);

    struct WaveSoundInfo;
    struct TrackInfo;
    struct NoteInfo;
    struct WaveSoundData {
        
        WaveSoundInfo* GetWaveSoundInfo() const;

        Util::ReferenceTable* GetTrackInfoReferenceTable() const;
        Util::ReferenceTable* GetNoteInfoReferenceTable() const;

        TrackInfo* GetTrackInfo(u32 index) const;
        NoteInfo* GetNoteInfo(u32 index) const;

        Util::Reference toWaveSoundInfo;
        Util::Reference toTrackInfoReferenceTable;
        Util::Reference toNoteInfoReferenceTable;
    };
    static_assert(sizeof(WaveSoundData) == 0x18);

    struct WaveSoundInfo {

        u8 GetPan() const;
        s8 GetSurroundPan() const;
        f32 GetPitch() const;
        void GetSendValue(u8* mainSend, u8* fxSend, u8 fxSendCount) const;
        AdshrCurve* GetAdshrCurve() const;
        u8 GetLpfFreq() const;
        u8 GetBiquadType() const;
        u8 GetBiquadValue() const;

        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(WaveSoundInfo) == 0x4);

    struct NoteEvent;
    struct TrackInfo {

        Util::ReferenceTable* GetNoteEventReferenceTable() const;
        NoteEvent* GetNoteEvent(u32 index) const;

        Util::Reference toNoteEventReferenceTable;
    };
    static_assert(sizeof(TrackInfo) == 0x8);

    struct NoteInfo {
        
        u8 GetOriginalKey() const;
        u8 GetVolume() const;
        u8 GetPan() const;
        u8 GetSurroundPan() const;
        f32 GetPitch() const;
        f32 GetSendValue(u8*, u8**, u8) const;
        AdshrCurve* GetAdshrCurve() const;

        u32 waveIdTableIndex;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(NoteInfo) == 0x8);
};
} // namespace nn::atk::detail