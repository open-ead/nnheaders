#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundMemoryAllocatable.h>

namespace nn::atk::detail {
struct LoadItemInfo {
    SoundArchive::ItemId itemId;
    void* address;
};
static_assert(sizeof(LoadItemInfo) == 0x10);

class SoundArchiveLoader {
public:
    enum LoadFlag {
        LoadFlag_All = -1,
        LoadFlag_Seq = 1,
        LoadFlag_Wsd = 2,
        LoadFlag_Bank = 4,
        LoadFlag_Warc = 8,
    };

    constexpr static s32 SignatureIndividualWave = 0x56574946; // FWIV
    constexpr static u8 WaveBufferAlignSize = 64;

    struct IndividualWaveInfo {
        u32 signature;
        u32 fileId;
        u32 waveIndex;
        u32 padding[5];
        u32 padding2[8];
    };
    static_assert(sizeof(IndividualWaveInfo) == 0x40);

    SoundArchiveLoader();
    virtual ~SoundArchiveLoader();
    
    virtual void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) = 0;
    virtual void* GetFileAddressFromTable(SoundArchive::FileId fileId) const = 0;
    virtual void* GetFileAddressImpl(SoundArchive::FileId fileId) const = 0;

    void SetSoundArchive(const SoundArchive* arc);

    bool IsAvailable() const;

    bool LoadData(SoundArchive::ItemId itemId, SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, size_t loadBlockSize);
    bool LoadSequenceSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator, 
                           u32 loadFlag, size_t loadBlockSize);
    bool LoadAdvancedWaveSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator, 
                               u32 loadFlag, size_t loadBlockSize);
    bool LoadWaveSound(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator, 
                       u32 loadFlag, size_t loadBlockSize, SoundArchive::ItemId waveSoundSetId);
    bool LoadStreamSoundPrefetch(SoundArchive::ItemId soundId, SoundMemoryAllocatable* pAllocator, 
                                 size_t loadBlockSize);
    bool LoadBank(SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, size_t loadBlockSize);
    bool LoadWaveArchive(SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator, 
                         u32 loadFlag, size_t loadBlockSize);
    bool LoadGroup(SoundArchive::ItemId soundGroupId, SoundMemoryAllocatable* pAllocator, 
                   size_t loadBlockSize);
    bool LoadSoundGroup(SoundArchive::ItemId soundGroupId, SoundMemoryAllocatable* pAllocator, 
                        u32 loadFlag, size_t loadBlockSize);

    bool LoadData(char* pItemName, SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, size_t loadBlockSize);
    void* LoadImpl(SoundArchive::FileId fileId, SoundMemoryAllocatable* pAllocator, 
                   size_t loadBlockSize, bool needMemoryPool);
    void* LoadFile(SoundArchive::FileId fileId, SoundMemoryAllocatable* pAllocator, 
                   size_t loadBlockSize, bool needMemoryPool);

    void* LoadWaveArchiveImpl(SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator, 
                              size_t loadBlockSize, bool needMemoryPool);
    bool LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex, SoundMemoryAllocatable* pAllocator, 
                            size_t loadBlockSize);
    void* LoadWaveArchiveTable(SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator, 
                               size_t loadBlockSize);

    size_t ReadFile(SoundArchive::FileId fileId, void* buffer, size_t size, s32 offset, 
                    size_t loadBlockSize);

    bool PostProcessForLoadedGroupFile(void* pGroupFile, SoundMemoryAllocatable* pAllocator,
                                       size_t loadBlockSize);

    void SetWaveArchiveTableWithSeqInEmbeddedGroup(SoundArchive::ItemId seqId, 
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableWithBankInEmbeddedGroup(SoundArchive::ItemId bankId, 
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableWithWsdInEmbeddedGroup(SoundArchive::ItemId wsdId, 
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableInEmbeddedGroupImpl(SoundArchive::ItemId warcId, 
                                                SoundMemoryAllocatable* pAllocator);

    bool IsDataLoaded(const char*, u32) const;
    bool IsDataLoaded(u32, u32) const;

    bool IsSequenceSoundDataLoaded(u32, u32) const;
    bool IsWaveSoundDataLoaded(u32, u32) const;
    bool IsBankDataLoaded(u32, u32) const;
    bool IsWaveArchiveDataLoaded(u32, u32) const;
    bool IsGroupDataLoaded(u32) const;
    bool IsSoundGroupDataLoaded(u32, u32) const;

    void* GetFileAddressFromSoundArchive(SoundArchive::FileId fileId) const;
    
    void* detail_GetFileAddressByItemId(SoundArchive::ItemId itemId) const;
    
    bool detail_LoadWaveArchiveByBankFile(void* bankFile, 
                                          SoundMemoryAllocatable* pAllocator);
    bool detail_LoadWaveArchiveByWaveSoundFile(void* wsdFile, s32 wsdIndex, 
                                               SoundMemoryAllocatable* pAllocator);

private:
    SoundArchive* m_pSoundArchive;
    u32 m_StreamArea[128];
    u16 m_LoadDataCallCount;
    bool m_IsCancelLoading;
};
static_assert(sizeof(SoundArchiveLoader) == 0x218);
} // namespace nn::atk::detail