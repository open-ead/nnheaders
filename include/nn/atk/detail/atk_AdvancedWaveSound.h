#pragma once

#include <nn/atk/detail/atk_BasicSound.h>
#include <nn/atk/detail/atk_SoundInstanceManager.h>
#include <nn/atk/detail/atk_AdvancedWaveSoundPlayer.h>

namespace nn::atk::detail {
class AdvancedWaveSound;
class AdvancedWaveSoundHandle;
using AdvancedWaveSoundInstanceManager = SoundInstanceManager<AdvancedWaveSound>;

class AdvancedWaveSound : BasicSound {
public:
    explicit AdvancedWaveSound(const AdvancedWaveSoundInstanceManager& manager);
    ~AdvancedWaveSound() override;

    bool Initialize(OutputReceiver* pOutputReceiver) override;
    void Finalize() override;

    void Prepare(const driver::AdvancedWaveSoundPlayer::PrepareParameter& parameter);

    void OnUpdatePlayerPriority() override;
    
    bool IsAttachedTempSpecialHandle() override;
    void DetachTempSpecialHandle() override;

    bool IsPrepared();

    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle();

private:
    friend AdvancedWaveSoundInstanceManager;

    util::IntrusiveListNode m_PriorityLink;
    AdvancedWaveSoundHandle* m_pTempSpecialHandle;
    AdvancedWaveSoundInstanceManager* m_InstanceManager;
    driver::AdvancedWaveSoundPlayer m_PlayerInstance;
    bool m_IsInitialized;
    u8 m_Padding[3];
};
static_assert(sizeof(AdvancedWaveSound) == 0x9b0);
} // namespace nn::atk::detail
