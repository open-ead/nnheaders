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

#if NN_SDK_VER < NN_MAKE_VER(5, 3, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void Prepare(const driver::AdvancedWaveSoundPlayer::PrepareParameter& parameter);

    void OnUpdatePlayerPriority() override;
    
    bool IsAttachedTempSpecialHandle() override;
    void DetachTempSpecialHandle() override;

    bool IsPrepared() const override;

    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() override;

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
