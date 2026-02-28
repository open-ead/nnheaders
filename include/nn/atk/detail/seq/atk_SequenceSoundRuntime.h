#pragma once

#include <nn/atk/detail/atk_SoundArchiveManager.h>
#include <nn/atk/detail/atk_StartInfoReader.h>
#include <nn/atk/detail/seq/atk_MmlSequenceTrackAllocator.h>
#include <nn/atk/detail/seq/atk_SequenceSound.h>
#include <nn/atk/detail/seq/atk_SequenceSoundFile.h>

namespace nn::atk::detail {
class SequenceSoundRuntime {
public:
    class SequenceNoteOnCallback : driver::NoteOnCallback {
    public:
        ~SequenceNoteOnCallback() override;

        driver::Channel* NoteOn(driver::SequenceSoundPlayer* seqPlayer, u8 bankIndex, 
                                const driver::NoteOnInfo& noteOnInfo) override;
    private:
        SequenceSoundRuntime* m_pSequenceSoundRuntime;
    };
    static_assert(sizeof(SequenceNoteOnCallback) == 0x10);

    struct PrepareContext {
        SequenceSoundFile* pSequenceSoundFile;
        u32 sequenceOffset;
        u32 allocateTrackFlags;
        LoadItemInfo loadTargetSequenceInfo;
        LoadItemInfo loadTargetBankInfos[4];
        LoadItemInfo loadTargetWaveArchiveInfos[4];
        bool isLoadIndividuals[4];
        bool canUsePlayerHeap;
        bool isRegisterDataLoadTaskNeeded;
    };
    static_assert(sizeof(PrepareContext) == 0xa8);

    SequenceSoundRuntime();
    ~SequenceSoundRuntime();

    void Initialize(s32 soundCount, void** pOutAllocatedAddr, const void* endAddr);
    void Finalize();

    void SetupSequenceTrack(s32 trackCount, void** pOutAllocatedAddr, const void* endAddr);
    void SetupUserParam(void** pOutAllocatedAddr, std::size_t adjustSize);

    static std::size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, 
                                             s32 alignment);
    static std::size_t GetRequiredSequenceTrackMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, 
                                                          s32 alignment);
    
    bool IsSoundArchiveAvailable() const;

    s32 GetActiveCount() const;
    s32 GetFreeCount() const;

    static void SetSequenceSkipIntervalTick(s32 tick);
    static s32 GetSequenceSkipIntervalTick();

    void Update();

    SequenceSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                              BasicSound::AmbientInfo* ambientArgInfo);
    SequenceSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                              BasicSound::AmbientInfo* ambientArgInfo, OutputReceiver* pOutputReceiver);

    SoundStartable::StartResult PrepareImpl(const SoundArchiveManager::SnapShot& snapShot, 
                                            SoundArchive::ItemId soundId, 
                                            SequenceSound* sound, 
                                            const SoundArchive::SoundInfo* commonInfo, 
                                            const StartInfoReader& startInfoReader);

    SoundStartable::StartResult SetupSequenceSoundInfo(SoundStartable::StartInfo::SequenceSoundInfo* sequenceSoundInfo, 
                                                       SoundArchive::ItemId soundId, 
                                                       const SoundArchive& soundArchive,
                                                       const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo);

    SoundStartable::StartResult SetupSequenceSoundFile(PrepareContext* pOutContext, 
                                                       const SequenceSound& sound, 
                                                       const SoundArchive& soundArchive,
                                                       const SoundDataManager& soundDataManager,
                                                       const SoundArchive::SoundInfo& commonInfo, 
                                                       const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo);

    SoundStartable::StartResult SetupBankFileAndWaveArchiveFile(PrepareContext* pOutContext, 
                                                                const SequenceSound& sound, 
                                                                const SoundStartable::StartInfo::SequenceSoundInfo& sequenceSoundInfo,
                                                                const SoundArchiveManager::SnapShot& snapShot,
                                                                const SoundStartable::StartInfo::SequenceSoundInfo* pExternalSequenceSoundInfo);

    void SetupSequenceSoundPlayerStartInfo(SoundStartable::StartInfo* startInfo, 
                                           SoundArchive::ItemId soundId, 
                                           const StartInfoReader& startInfoReader);

    void DumpMemory(const SoundArchive*) const;

    void SetupBankFileAndWaveArchiveFileFromHook(PrepareContext* pOutContext, 
                                                 SequenceSound* sound, 
                                                 SoundArchive* soundArchive);


private:
    SequenceSoundInstanceManager m_SequenceSoundInstanceManager;
    driver::SequenceSoundLoaderManager m_SequenceSoundLoaderManager;
    driver::SequenceTrackAllocator* m_pSequenceTrackAllocator;
    driver::MmlSequenceTrackAllocator m_MmlSequenceTrackAllocator;
    driver::MmlParser m_MmlParser;
    SequenceNoteOnCallback m_SequenceCallback;
    SequenceUserProcCallback m_SequenceUserProcCallback;
    void* m_pSequenceUserProcCallbackArg;
    SoundArchiveManager* m_pSoundArchiveManager;
    SoundArchiveFilesHook* m_pSoundArchiveFilesHook; 
};
static_assert(sizeof(SequenceSoundRuntime) == 0xe8);
}  // namespace nn::atk::detail
