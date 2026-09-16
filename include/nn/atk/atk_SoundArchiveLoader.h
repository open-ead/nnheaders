#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundMemoryAllocatable.h>

namespace nn::atk::detail {

struct LoadItemInfo {
    SoundArchive::ItemId itemId;
    const void* address;

    LoadItemInfo() = default;
};
static_assert(sizeof(LoadItemInfo) == 0x10);

class SoundArchiveLoader {
public:
    static const u32 SignatureIndividualWave{0x56574946};  // FWIV
    static const int WaveBufferAlignSize{64};

    struct IndividualWaveInfo {
        u32 signature{SignatureIndividualWave};
        u32 fileId;
        u32 waveIndex;
        u32 padding[5];
        u32 padding2[8];

        IndividualWaveInfo(u32 _fileId, u32 _waveIndex) : fileId{_fileId}, waveIndex{_waveIndex} {};
    };
    static_assert(sizeof(IndividualWaveInfo) == 0x40);

    enum LoadFlag {
        LoadFlag_Seq = 1 << 0,
        LoadFlag_Wsd = 1 << 1,
        LoadFlag_Bank = 1 << 2,
        LoadFlag_Warc = 1 << 3,
        LoadFlag_All = -1,
    };

    SoundArchiveLoader();
    virtual ~SoundArchiveLoader();

    bool IsAvailable() const;

    bool LoadData(SoundArchive::ItemId itemId, SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                  size_t loadBlockSize);

    bool LoadData(const char* pItemName, SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                  size_t loadBlockSize);

    bool IsDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const;

    bool IsDataLoaded(const char* pItemName, u32 loadFlag) const;

    bool CancelLoading() { return m_IsCancelLoading; }

    const void* detail_GetFileAddressByItemId(SoundArchive::ItemId itemId) const;

    bool detail_LoadWaveArchiveByBankFile(const void* bankFile, SoundMemoryAllocatable* pAllocator);

    bool detail_LoadWaveArchiveByWaveSoundFile(const void* wsdFile, int wsdIndex,
                                               SoundMemoryAllocatable* pAllocator);

protected:
    void SetSoundArchive(const SoundArchive* arc);
    const SoundArchive* GetSoundArchive() const { return m_pSoundArchive; }

    const void* GetFileAddressFromSoundArchive(SoundArchive::FileId fileId) const;

    virtual const void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) = 0;
    virtual const void* GetFileAddressFromTable(SoundArchive::FileId fileId) const = 0;
    virtual const void* GetFileAddressImpl(SoundArchive::FileId fileId) const = 0;

private:
    bool LoadSequenceSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator,
                           u32 loadFlag, size_t loadBlockSize);

    bool LoadWaveSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator,
                       u32 loadFlag, size_t loadBlockSize, SoundArchive::ItemId waveSoundSetId);

    bool LoadAdvancedWaveSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator,
                               u32 loadFlag, size_t loadBlockSize);

    bool LoadStreamSoundPrefetch(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator,
                                 size_t loadBlockSize);

    bool LoadBank(SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                  size_t loadBlockSize);

    bool LoadWaveArchive(SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator,
                         u32 loadFlag, size_t loadBlockSize);

    const void* LoadWaveArchiveTable(SoundArchive::ItemId warcId,
                                     SoundMemoryAllocatable* pAllocator, size_t loadBlockSize);

    bool LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex,
                            SoundMemoryAllocatable* pAllocator, size_t loadBlockSize);

    bool LoadGroup(SoundArchive::ItemId groupId, SoundMemoryAllocatable* pAllocator,
                   size_t loadBlockSize);

    bool LoadSoundGroup(SoundArchive::ItemId soundGroupId, SoundMemoryAllocatable* pAllocator,
                        u32 loadFlag, size_t loadBlockSize);

    const void* LoadImpl(SoundArchive::FileId fileId, SoundMemoryAllocatable* pAllocator,
                         size_t loadBlockSize, bool needMemoryPool);

    void* LoadWaveArchiveImpl(SoundArchive::ItemId warcId, u32 waveIndex,
                              SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                              size_t loadBlockSize);

    bool PostProcessForLoadedGroupFile(const void* pGroupFile, SoundMemoryAllocatable* pAllocator,
                                       size_t loadBlockSize);

    bool IsSequenceSoundDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const;
    bool IsWaveSoundDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const;
    bool IsBankDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const;
    bool IsWaveArchiveDataLoaded(SoundArchive::ItemId itemId, u32 waveIndex) const;
    bool IsGroupDataLoaded(SoundArchive::ItemId itemId) const;
    bool IsSoundGroupDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const;

    void* LoadFile(SoundArchive::FileId fileId, SoundMemoryAllocatable* allocator,
                   size_t loadBlockSize, bool needMemoryPool);

    size_t ReadFile(SoundArchive::FileId fileId, void* buffer, size_t size, int offset,
                    size_t loadBlockSize);

    void SetWaveArchiveTableWithSeqInEmbeddedGroup(SoundArchive::ItemId seqId,
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableWithBankInEmbeddedGroup(SoundArchive::ItemId bankId,
                                                    SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableWithWsdInEmbeddedGroup(SoundArchive::ItemId wsdId,
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableInEmbeddedGroupImpl(SoundArchive::ItemId warcId,
                                                SoundMemoryAllocatable* pAllocator);

    const SoundArchive* m_pSoundArchive{};
    u32 m_StreamArea[128];
    u16 m_LoadDataCallCount;
    bool m_IsCancelLoading;
};
static_assert(sizeof(SoundArchiveLoader) == 0x218);

}  // namespace nn::atk::detail
