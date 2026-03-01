#pragma once

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
    virtual void unk1() = 0;
    virtual void unk2() = 0;
    virtual void unk3() = 0;
    virtual void unk4() = 0;
    virtual void unk5() = 0;
    virtual void unk6() = 0;
    virtual void unk7() = 0;
    virtual void UpdateBuffer(s32, void**, std::size_t, SampleFormat, s32, OutputMode);
    virtual void GetChannelIndex(ChannelIndex* pChannel, s32 channelCount) const;
    virtual s32 GetChannelSettingCountMax() const;
    virtual void OnChangeOutputMode();
    virtual void SetEffectBuffer(void* effectBuffer, std::size_t effectBufferSize);

    bool AddEffect(audio::AudioRendererConfig* pConfig, audio::FinalMixType* pFinalMixType);
    bool AddEffect(audio::AudioRendererConfig* pConfig, audio::SubMixType* pFinalMixType);

    void SetEffectInputOutput(const s8* input, const s8* output, s32 inputCount, s32 outputCount);

    void RemoveEffect(audio::AudioRendererConfig* pConfig, audio::FinalMixType* pFinalMixType);
    void RemoveEffect(audio::AudioRendererConfig* pConfig, audio::SubMixType* pSubMixType);

    static s32 ConvertChannelModeToInt(ChannelMode channelMode);

    SampleRate GetSampleRate() const;
    bool SetSampleRate(SampleRate sampleRate);

private:
    friend OutputMixer;

    util::IntrusiveListNode m_Link;
    bool m_IsActive;
    SampleRate m_SampleRate;
    void* m_EffectBuffer;
    std::size_t m_EffectBufferSize;
};
static_assert(sizeof(EffectBase) == 0x30);
} // namespace nn::atk