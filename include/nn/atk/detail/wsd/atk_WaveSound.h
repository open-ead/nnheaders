#pragma once

#include <nn/atk/atk_WaveSoundHandle.h>
#include <nn/atk/detail/atk_BasicSound.h>
#include <nn/atk/detail/atk_SoundInstanceManager.h>
#include <nn/atk/detail/wsd/atk_WaveSoundPlayer.h>

namespace nn::atk::detail { 
class WaveSound;
using WaveSoundInstanceManager = SoundInstanceManager<WaveSound>;

class WaveSound : BasicSound {
public:
    explicit WaveSound(WaveSoundInstanceManager& manager);
    ~WaveSound() override;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void Prepare(const void* wsdFile, const void* waveFile, 
                 const driver::WaveSoundPlayer::StartInfo& startInfo, s8 waveType);

    void RegisterDataLoadTask(const driver::WaveSoundLoader::LoadInfo& loadInfo, 
                              const driver::WaveSoundPlayer::StartInfo& startInfo);

    void SetChannelPriority(s32 priority);

    void InitializeChannelParam(s32 priority, bool isReleasePriorityFix);

    void OnUpdatePlayerPriority() override;
    
    bool IsAttachedTempSpecialHandle() override;
    void DetachTempSpecialHandle() override;

    bool ReadWaveSoundDataInfo(WaveSoundDataInfo*) const;

    position_t GetPlaySamplePosition(bool) const;

    bool IsPrepared() const override;

    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() override;

    void OnUpdateParam() override;

private:
    friend WaveSoundInstanceManager;

    util::IntrusiveListNode m_PriorityLink;
    WaveSoundHandle* m_pTempSpecialHandle;
    WaveSoundInstanceManager* m_Manager;
    void* m_pWaveFile;
    s8 m_WaveType;
    bool m_InitializeFlag;
    bool m_IsCalledPrepare;
    u8 m_Padding[1];
    u32 m_ChannelCount;
    driver::WaveSoundPlayer m_PlayerInstance;
};
static_assert(sizeof(WaveSound) == 0x3e0);
} // namespace nn::atk::detail