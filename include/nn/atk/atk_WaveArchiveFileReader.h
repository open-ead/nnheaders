#pragma once

#include <nn/atk/atk_WaveArchiveFile.h>

namespace nn::atk::detail {

class WaveArchiveFileReader {
public:
    static const u32 SignatureFile = 0x52415746;       // FWAR
    static const u32 SignatureWarcTable = 0x54415746;  // FWAT

    WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual);
    WaveArchiveFileReader();

    void Initialize(const void* pWaveArchiveFile, bool isIndividual);
    void Finalize();

    void InitializeFileTable();

    bool IsAvailable() const { return m_pHeader != nullptr; }

    u32 GetWaveFileCount() const;
    u32 GetWaveFileSize(u32 waveIndex) const;
    u32 GetWaveFileOffsetFromFileHead(u32 waveIndex) const;

    const void* GetWaveFile(u32 waveIndex) const;
    const void* SetWaveFile(u32 waveIndex, const void* pWaveFile);

    bool IsLoaded(u32 waveIndex) { return m_IsInitialized && GetWaveFile(waveIndex) != nullptr; }

    bool HasIndividualLoadTable() const;

    struct IndividualLoadTable {
        const void* waveFile[1];
    };

private:
    const void* GetWaveFileForWhole(u32 waveIndex) const {
        u32 offset{m_pInfoBlockBody->GetOffsetFromFileBlockBody(waveIndex)};

        return util::ConstBytePtr(&m_pHeader->GetFileBlock()->body, offset).Get();
    }

    const void* GetWaveFileForIndividual(u32 waveIndex) const {
        return m_pLoadTable->waveFile[waveIndex];
    }

    const WaveArchiveFile::FileHeader* m_pHeader{};
    const WaveArchiveFile::InfoBlockBody* m_pInfoBlockBody{};
    IndividualLoadTable* m_pLoadTable{};
    bool m_IsInitialized{false};
};
static_assert(sizeof(WaveArchiveFileReader) == 0x20);

}  // namespace nn::atk::detail