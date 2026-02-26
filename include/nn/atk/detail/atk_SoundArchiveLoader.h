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

    void SetSoundArchive(SoundArchive* arc);

    bool LoadData(SoundArchive::ItemId itemId, 
                  SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, std::size_t loadBlockSize);
    bool LoadSequenceSound(SoundArchive::ItemId soundId, 
                           SoundMemoryAllocatable* pAllocator, 
                           u32 loadFlag, std::size_t loadBlockSize);
    bool LoadAdvancedWaveSound(SoundArchive::ItemId soundId, 
                               SoundMemoryAllocatable* pAllocator, 
                               u32 loadFlag, std::size_t loadBlockSize);
    bool LoadWaveSound(SoundArchive::ItemId soundId, 
                       SoundMemoryAllocatable* pAllocator, 
                       u32 loadFlag, std::size_t loadBlockSize,
                       SoundArchive::ItemId waveSoundSetId);
    bool LoadBank(SoundArchive::ItemId bankId, 
                  SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, std::size_t loadBlockSize);
    bool LoadSoundGroup(SoundArchive::ItemId soundGroupId, 
                        SoundMemoryAllocatable* pAllocator, 
                        u32 loadFlag, std::size_t loadBlockSize);
    bool LoadData(char* pItemName, SoundMemoryAllocatable* pAllocator, 
                  u32 loadFlag, std::size_t loadBlockSize);
    void* LoadImpl(SoundArchive::FileId fileId, 
                   SoundMemoryAllocatable* pAllocator, 
                   std::size_t loadBlockSize, bool needMemoryPool);
    bool LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex,
                            SoundMemoryAllocatable* pAllocator, 
                            std::size_t loadBlockSize);
    void* LoadWaveArchiveTable(SoundArchive::ItemId warcId,
                               SoundMemoryAllocatable* pAllocator, 
                               std::size_t loadBlockSize);

    std::size_t ReadFile(SoundArchive::FileId fileId, void* buffer, 
                         std::size_t size, s32 offset, 
                         std::size_t loadBlockSize);

    bool PostProcessForLoadedGroupFile(void* pGroupFile, 
                                       SoundMemoryAllocatable* pAllocator,
                                       std::size_t loadBlockSize);

    void SetWaveArchiveTableWithSeqInEmbeddedGroup(SoundArchive::ItemId seqId, 
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableWithWsdInEmbeddedGroup(SoundArchive::ItemId wsdId, 
                                                   SoundMemoryAllocatable* pAllocator);
    void SetWaveArchiveTableInEmbeddedGroupImpl(SoundArchive::ItemId warcId, 
                                                SoundMemoryAllocatable* pAllocator);

    void* GetFileAddressFromSoundArchive(SoundArchive::FileId fileId);
    
    void* detail_GetFileAddressByItemId(SoundArchive::ItemId itemId);
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