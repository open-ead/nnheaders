#pragma once

#include <nn/atk/detail/seq/atk_BankFile.h>

namespace nn::atk::detail {
class BankFileReader {
public:
    constexpr static s32 SignatureFile = 0x4b4e4246; // FBNK

    struct VelocityRegionInfo {
        u32 waveArchiveId;
        u32 waveIndex;
        f32 pitch;
        AdshrCurve adshrCurve;
        u8 originalKey;
        u8 volume;
        u8 pan;
        bool isIgnoreNoteOff;
        u8 keyGroup;
        u8 interpolationType;
    };
    static_assert(sizeof(VelocityRegionInfo) == 0x18);

    BankFileReader();
    explicit BankFileReader(const void* bankFile);

    void Initialize(const void* bankFile);
    void Finalize();

    bool ReadVelocityRegionInfo(VelocityRegionInfo*  info, s32 programNo, 
                                s32 key, s32 velocity) const;
    
    Util::WaveIdTable* GetWaveIdTable() const;

private:
    BankFile::FileHeader* m_pHeader;
    BankFile::InfoBlockBody* m_pInfoBlockBody;
    bool m_IsInitialized;
};
static_assert(sizeof(BankFileReader) == 0x18);
} // namespace nn::atk::detail