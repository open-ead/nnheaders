#pragma once

#include <nn/atk/detail/atk_WaveArchiveFile.h>

namespace nn::atk::detail {
class WaveArchiveFileReader {
public:
    constexpr static s32 SignatureFile = 0x52415746; // FWAR
    constexpr static s32 SignatureWarcTable = 0x54415746; // FWAT

    struct IndividualLoadTable {
        void* waveFile[1];
    };

    WaveArchiveFileReader();
    WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual);
    
    void Initialize(const void* pWaveArchiveFile, bool isIndividual);

    bool HasIndividualLoadTable() const;

    void Finalize();

    void InitializeFileTable();

    u32 GetWaveFileCount() const;
    void* GetWaveFile(u32 waveIndex) const;
    u32 GetWaveFileSize(u32 waveIndex) const;
    u32 GetWaveFileOffsetFromFileHead(u32 waveIndex) const;

    void* SetWaveFile(u32 waveIndex, const void* pWaveFile);

private:
    WaveArchiveFile::FileHeader* m_pHeader;
    WaveArchiveFile::InfoBlockBody* m_pInfoBlockBody;
    IndividualLoadTable* m_pLoadTable;
    bool m_IsInitialized;
};
static_assert(sizeof(WaveArchiveFileReader) == 0x20);
} // namespace nn::atk::detail