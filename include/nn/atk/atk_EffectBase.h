#pragma once

#include <nn/util.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_SubMixTypes.h>

#include <nn/atk/atk_Global.h>

namespace nn::atk {

class OutputMixer;

class EffectBase {
public:
    enum ChannelMode {
        ChannelMode_1Ch,
        ChannelMode_2Ch,
        ChannelMode_4Ch,
        ChannelMode_6Ch,
    };

    enum SampleRate {
        SampleRate_32000,
        SampleRate_48000,
    };

    constexpr static u8 ChannelModeCountMax = 6;

    EffectBase();

    virtual ~EffectBase();
    virtual size_t GetRequiredMemSize() const = 0;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual bool AddEffect(audio::AudioRendererConfig* pConfig,
                           audio::FinalMixType* pFinalMixType) = 0;
    virtual bool AddEffect(audio::AudioRendererConfig* pConfig,
                           audio::SubMixType* pFinalMixType) = 0;
#else
    virtual bool AddEffect(audio::AudioRendererConfig* pConfig, OutputMixer* pOutputMixer) = 0;
#endif
    virtual void SetEffectInputOutput(const s8* input, const s8* output, int inputCount,
                                      int outputCount) = 0;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual void RemoveEffect(audio::AudioRendererConfig* pConfig,
                              audio::FinalMixType* pFinalMixType) = 0;
    virtual void RemoveEffect(audio::AudioRendererConfig* pConfig,
                              audio::SubMixType* pSubMixType) = 0;
#else
    virtual void RemoveEffect(audio::AudioRendererConfig* pConfig, OutputMixer* pOutputMixer) = 0;
#endif
    virtual bool IsRemovable() const = 0;
    virtual void UpdateBuffer(int, void**, size_t, SampleFormat, int, OutputMode);

    SampleRate GetSampleRate() const;
    bool SetSampleRate(SampleRate sampleRate);

    virtual void GetChannelIndex(ChannelIndex* pChannel, int channelCount) const;
    virtual int GetChannelSettingCountMax() const;
    virtual void OnChangeOutputMode();
    virtual void SetEffectBuffer(void* effectBuffer, size_t effectBufferSize);

protected:
    static int ConvertChannelModeToInt(ChannelMode channelMode);

private:
    NN_NO_COPY(EffectBase);

    friend OutputMixer;

    util::IntrusiveListNode m_Link;
    bool m_IsActive;
    SampleRate m_SampleRate;
    void* m_EffectBuffer;
    size_t m_EffectBufferSize;
};
static_assert(sizeof(EffectBase) == 0x30);

}  // namespace nn::atk
