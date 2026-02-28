#pragma once

#include <nn/atk/detail/atk_StartInfoReader.h>
#include <nn/atk/detail/wsd/atk_WaveSound.h>

namespace nn::atk::detail {
class WaveSoundRuntime {
public:
    WaveSoundRuntime();
    ~WaveSoundRuntime();

    void Initialize(s32 soundCount, void** pOutAllocatedAddr, const void* endAddr);
    void Finalize();

    static std::size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, 
                                             s32 alignment);

    s32 GetActiveCount() const;
    s32 GetFreeWaveSoundCount() const;

    void SetupUserParam(void** startAddr, std::size_t adjustSize);

    void Update();
    
    WaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                          BasicSound::AmbientInfo* ambientArgInfo);
    WaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                          BasicSound::AmbientInfo* ambientArgInfo, OutputReceiver* pOutputReceiver);

    SoundStartable::StartResult PrepareImpl(SoundArchive* pSoundArchive, 
                                            SoundDataManager* pSoundDataManager, 
                                            SoundArchive::ItemId soundId, 
                                            WaveSound* sound, 
                                            SoundArchive::SoundInfo* commonInfo, 
                                            StartInfoReader* startInfoReader);

    void DumpMemory(const SoundArchive*);

private:
    WaveSoundInstanceManager m_WaveSoundInstanceManager;
    driver::WaveSoundLoaderManager m_WaveSoundLoaderManager;
    SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
};
static_assert(sizeof(WaveSoundRuntime) == 0x88);
}  // namespace nn::atk::detail
