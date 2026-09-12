#pragma once

#include <nn/atk/atk_BankFile.h>

namespace nn::atk::detail {

struct VelocityRegionInfo {
    u32 waveArchiveId;
    u32 waveIndex;
    float pitch;
    AdshrCurve adshrCurve;
    u8 originalKey;
    u8 volume;
    u8 pan;
    bool isIgnoreNoteOff;
    u8 keyGroup;
    u8 interpolationType;

    VelocityRegionInfo() : adshrCurve(0, 0, 0, 0, 0) {};
};
static_assert(sizeof(VelocityRegionInfo) == 0x18);

class BankFileReader {
public:
    static const u32 SignatureFile{0x4b4e4246};  // FBNK

    BankFileReader();
    explicit BankFileReader(const void* bankFile);

    void Initialize(const void* bankFile);
    void Finalize();

    bool IsInitialized() const { return m_IsInitialized; }

    bool ReadVelocityRegionInfo(VelocityRegionInfo* info, int programNo, int key,
                                int velocity) const;

    const Util::WaveIdTable* GetWaveIdTable() const;

    const void* GetBankFileAddress() const { return m_pHeader; }

    int GetInstrumentCount() const { return m_pInfoBlockBody->GetInstrumentCount(); }

private:
    const BankFile::FileHeader* m_pHeader{};
    const BankFile::InfoBlockBody* m_pInfoBlockBody{};
    bool m_IsInitialized{false};
};
static_assert(sizeof(BankFileReader) == 0x18);

}  // namespace nn::atk::detail