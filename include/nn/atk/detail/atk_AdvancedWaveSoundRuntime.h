#pragma once

#include <nn/types.h>

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/detail/atk_AdvancedWaveSound.h>
#include <nn/atk/detail/atk_StartInfoReader.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk::detail {
class AdvancedWaveSoundRuntime {
public:
    AdvancedWaveSoundRuntime();
    ~AdvancedWaveSoundRuntime();

    void Initialize(s32 soundCount, void** pOutAllocatedAddr, const void* endAddr);
    void Finalize();

    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo,
                                        size_t alignmentSize);

    s32 GetActiveCount() const;
    s32 GetFreeAdvancedWaveSoundCount() const;

    void SetupUserParam(void** startAddr, size_t adjustSize);
    
    void Update();

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    AdvancedWaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, 
                                  s32 ambientPriority, BasicSound::AmbientInfo* ambientArgInfo);
#else
    AdvancedWaveSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, 
                                  s32 ambientPriority, BasicSound::AmbientInfo* ambientArgInfo, 
                                  OutputReceiver* pOutputReceiver);
#endif

    SoundStartable::StartResult PrepareImpl(const SoundArchive* pSoundArchive, 
                                            const SoundDataManager* pSoundDataManager, 
                                            SoundArchive::ItemId soundId, 
                                            AdvancedWaveSound* sound, 
                                            const SoundArchive::SoundInfo* commonInfo, 
                                            const StartInfoReader& startInfoReader);

    void DumpMemory(const SoundArchive*) const;

private:
    AdvancedWaveSoundInstanceManager m_InstanceManager;
};
static_assert(sizeof(AdvancedWaveSoundRuntime) == 0x38);
}  // namespace nn::atk::detail
