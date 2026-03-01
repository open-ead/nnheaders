#pragma once

#include <atomic>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>

#include <nn/atk/submix/atk_OutputMixer.h>

namespace nn::atk {
class FinalMix : OutputMixer {
public:
    static size_t GetRequiredMemorySize(bool isEffectEnabled);

    bool Initialize(audio::AudioRendererConfig* pConfig, s32 channelCount, 
                    bool isEffectEnabled, void* buffer, size_t bufferSize);

    void Finalize(audio::AudioRendererConfig* pConfig);

    bool AppendEffect(EffectBase* pEffect, void* buffer, size_t bufferSize);
    bool AppendEffect(EffectAux* pEffect, void* buffer, size_t bufferSize);

    bool RemoveEffect(EffectBase* pEffect);
    bool RemoveEffect(EffectAux* pEffect);

    void ClearEffect();

    bool IsEffectEnabled() const;

    ReceiverType GetReceiverType() const override;
    s32 GetChannelCount() const override;
    s32 GetBusCount() const override;

    void AddReferenceCount(s32 value) override;

    bool IsSoundSendClampEnabled(s32 bus) const override;

private:
    audio::FinalMixType m_FinalMix;
    std::atomic_uint m_ReferenceCount;
    s32 m_ChannelCount;
};
static_assert(sizeof(FinalMix) == 0x50);
} // namespace nn::atk