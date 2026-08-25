#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct WaveSoundFile {
    struct InfoBlock;
    struct FileHeader : Util::SoundFileHeader {
        const InfoBlock* GetInfoBlock() const;
    };

    struct WaveSoundData;
    struct InfoBlockBody {
        Util::Reference toWaveIdTable;
        Util::Reference toWaveSoundDataReferenceTable;

        const Util::WaveIdTable& GetWaveIdTable() const;
        const Util::ReferenceTable& GetWaveSoundDataReferenceTable() const;
        
        u32 GetWaveIdCount() const {
            return GetWaveIdTable().GetCount();
        }

        u32 GetWaveSoundCount() const {
            return GetWaveSoundDataReferenceTable().count;
        }

        const Util::WaveId* GetWaveId(u32 index) const {
            return GetWaveIdTable().GetWaveId(index);
        }

        const WaveSoundData& GetWaveSoundData(u32 index) const;
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
        Util::Reference toWaveSoundInfo;
        Util::Reference toTrackInfoReferenceTable;
        Util::Reference toNoteInfoReferenceTable;

        const WaveSoundInfo& GetWaveSoundInfo() const;

        const Util::ReferenceTable& GetTrackInfoReferenceTable() const;
        const Util::ReferenceTable& GetNoteInfoReferenceTable() const;

        u32 GetTrackCount() const {
            return GetTrackInfoReferenceTable().count;
        }

        u32 GetNoteCount() const {
            return GetNoteInfoReferenceTable().count;
        }

        const TrackInfo& GetTrackInfo(u32 index) const;
        const NoteInfo& GetNoteInfo(u32 index) const;
    };
    static_assert(sizeof(WaveSoundData) == 0x18);

    struct WaveSoundInfo {
        Util::BitFlag optionParameter;

        u8 GetPan() const;
        s8 GetSurroundPan() const;
        float GetPitch() const;
        void GetSendValue(u8* mainSend, u8* fxSend, u8 fxSendCount) const;
        const AdshrCurve& GetAdshrCurve() const;
        u8 GetLpfFreq() const;
        u8 GetBiquadType() const;
        u8 GetBiquadValue() const;
    };
    static_assert(sizeof(WaveSoundInfo) == 0x4);

    struct NoteEvent;
    struct TrackInfo {
        Util::Reference toNoteEventReferenceTable;

        const Util::ReferenceTable& GetNoteEventReferenceTable() const;

        u32 GetNoteEventCount() const {
            return GetNoteEventReferenceTable().count;
        }

        const NoteEvent& GetNoteEvent(u32 index) const;
    };
    static_assert(sizeof(TrackInfo) == 0x8);

    struct NoteEvent {
        u8 _0[8];
    };
    static_assert(sizeof(NoteEvent) == 0x8);

    struct NoteInfo {
        u32 waveIdTableIndex;
        Util::BitFlag optionParameter;

        u8 GetOriginalKey() const;
        u8 GetVolume() const;
        u8 GetPan() const;
        u8 GetSurroundPan() const;
        float GetPitch() const;
        void GetSendValue(u8* mainSend, u8** fxSend, u8 fxSendCount) const;
        const AdshrCurve& GetAdshrCurve() const;
    };
    static_assert(sizeof(NoteInfo) == 0x8);
};
} // namespace nn::atk::detail