#pragma once

#include <nn/atk/detail/atk_StartInfoReader.h>
#include <nn/atk/detail/wsd/atk_WaveSound.h>

namespace nn::atk::detail {
class WaveSoundRuntime {
public:
    WaveSoundRuntime();
    ~WaveSoundRuntime();

    bool Initialize(s32 soundCount, void** pOutAllocatedAddr, const void* endAddr);
    void Finalize();

    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo, 
                                        s32 alignment);

    s32 GetActiveCount() const;
    s32 GetFreeWaveSoundCount() const;

    void SetupUserParam(void** startAddr, size_t adjustSize);

    void Update();
    
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    WaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                          BasicSound::AmbientInfo* ambientArgInfo);
#else
    WaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                          BasicSound::AmbientInfo* ambientArgInfo, OutputReceiver* pOutputReceiver);
#endif

    SoundStartable::StartResult PrepareImpl(const SoundArchive* pSoundArchive, 
                                            const SoundDataManager* pSoundDataManager, 
                                            SoundArchive::ItemId soundId, 
                                            WaveSound* sound, 
                                            const SoundArchive::SoundInfo* commonInfo, 
                                            const StartInfoReader& startInfoReader);

    void DumpMemory(const SoundArchive*) const;

private:
    WaveSoundInstanceManager m_WaveSoundInstanceManager;
    driver::WaveSoundLoaderManager m_WaveSoundLoaderManager;
    SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(WaveSoundRuntime) == 0x80);
#else
static_assert(sizeof(WaveSoundRuntime) == 0x88);
#endif
}  // namespace nn::atk::detail
