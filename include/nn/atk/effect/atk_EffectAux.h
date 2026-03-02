#pragma once

#include <atomic>

#include <nn/util.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_EffectTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_SubMixTypes.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_Global.h>

namespace nn::atk {
class OutputMixer;

class EffectAux {
public:
    constexpr static s32 ChannelCountMax = 6;

    struct BufferSet {
        void* sendBuffer;
        void* returnBuffer;
        void* readBuffer;
    };
    static_assert(sizeof(BufferSet) == 0x18);

    struct UpdateSamplesArg {
        s32 sampleCountPerAudioFrame;
        s32 sampleRate;
        s32 audioFrameCount;
        s32 channelCount;
        s32 readSampleCount;
        OutputMode outputMode;
    };
    static_assert(sizeof(UpdateSamplesArg) == 0x18);

    EffectAux();
    virtual ~EffectAux();
    virtual void Initialize();
    virtual void Finalize();
    virtual void OnChangeOutputMode();
    virtual void UpdateSamples(s32, const UpdateSamplesArg&);

    void ResetChannelIndex();

    std::size_t GetRequiredMemSize(const audio::AudioRendererParameter& parameter) const;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool AddEffect(audio::AudioRendererConfig* pConfig, 
                   const audio::AudioRendererParameter& parameter, 
                   audio::FinalMixType* pFinalMixType);
    bool AddEffect(audio::AudioRendererConfig* pConfig, 
                   const audio::AudioRendererParameter& parameter, 
                   audio::SubMixType* pSubMixType);
#else
    bool AddEffect(audio::AudioRendererConfig* pConfig, 
                   const audio::AudioRendererParameter& parameter, 
                   OutputMixer* pOutputMixer);
#endif

    void SplitEffectBuffer(BufferSet* pBufferSet, void* effectBuffer, 
                           std::size_t effectBufferSize);

    void SetEffectInputOutput(const s8* input, const s8* output, s32 inputCount, s32 outputCount);

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    void RemoveEffect(audio::AudioRendererConfig* pConfig, audio::FinalMixType* pFinalMixType);
    void RemoveEffect(audio::AudioRendererConfig* pConfig, audio::SubMixType* pSubMixType);
#else
    void RemoveEffect(audio::AudioRendererConfig* pConfig, OutputMixer* pOutputMixer);
#endif

    bool SetChannelCount(s32 channelCount);
    bool SetChannelIndex(const ChannelIndex* pChannel, s32 channelCount);

    s32 GetChannelCount() const;
    void GetChannelIndex(ChannelIndex* pChannel, s32 channelCount) const;

    bool SetAudioFrameCount(s32 audioFrameCount);
    s32 GetAudioFrameCount() const;

    bool IsRemovable() const;
    bool IsClearable();
    bool IsEnabled() const;

    void SetEnabled(bool isEnabled);
    void SetEffectBuffer(void* effectBuffer, std::size_t effectBufferSize);

    void Update();

private:
    friend OutputMixer;

    util::IntrusiveListNode m_AuxLinkNode;
    audio::AuxType m_AuxType;
    std::atomic_uint64_t m_AudioRendererUpdateCountWhenAddedAux;
    s32 m_AudioFrameCount;
    s32 m_ChannelCount;
    bool m_IsActive;
    bool m_IsEnabled;
    void* m_EffectBuffer;
    std::size_t m_EffectBufferSize;
    s32* m_AuxReadBuffer;
    ChannelIndex m_ChannelSetting[ChannelCountMax];
};
static_assert(sizeof(EffectAux) == 0x50 + sizeof(ChannelIndex) * EffectAux::ChannelCountMax);
} // namespace nn::atk