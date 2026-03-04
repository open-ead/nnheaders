#pragma once

#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct BankFile {
    struct InfoBlock;
    struct FileHeader : Util::SoundFileHeader {

        InfoBlock* GetInfoBlock() const;
        
    };

    struct Instrument;
    struct InfoBlockBody {

        Util::WaveIdTable* GetWaveIdTable() const;
        Util::ReferenceTable GetInstrumentReferenceTable() const;
        Instrument* GetInstrument(s32 programNo) const;

        Util::Reference toWaveIdTable;
        Util::Reference toInstrumentReferenceTable;
    };
    static_assert(sizeof(InfoBlockBody) == 0x10);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x18);

    struct KeyRegion;
    struct Instrument {

        KeyRegion* GetKeyRegion(u32 key) const;

        Util::Reference toKeyRegionChunk;
    };
    static_assert(sizeof(Instrument) == 0x8);

    struct VelocityRegion;
    struct KeyRegion {

        VelocityRegion* GetVelocityRegion(u32 velocity) const;

        Util::Reference toVelocityRegionChunk;
    };
    static_assert(sizeof(KeyRegion) == 0x8);

    struct RegionParameter;
    struct VelocityRegion {

        u8 GetOriginalKey() const;
        u8 GetVolume() const;
        u8 GetPan() const;
        f32 GetPitch() const ;
        bool IsIgnoreNoteOff() const;
        u8 GetKeyGroup() const;
        u8 GetInterpolationType() const;
        AdshrCurve* GetAdshrCurve() const;
        RegionParameter* GetRegionParameter() const;

        u32 waveIdTableIndex;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(VelocityRegion) == 0x8);

    struct RegionParameter {
        u8 originalKey;
        u8 padding1[3];
        u8 volume;
        u8 padding2[3];
        u8 pan;
        s8 surroundPan;
        u8 padding3[2];
        f32 pitch;
        bool isIgnoreNoteOff;
        u8 keyGroup;
        u8 interpolationType;
        u8 padding4[1];
        u32 offset;
        Util::Reference refToAdshrCurve;
        AdshrCurve adshrCurve;
    };
    static_assert(sizeof(RegionParameter) == 0x28);
};
} // namespace nn::atk::detail