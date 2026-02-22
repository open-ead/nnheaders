#pragma once

#include <nn/types.h>

namespace nn::audio {
struct VoiceInfo;

struct VoiceType {
    enum PlayState {
        PlayState_Play,
        PlayState_Stop,
        PlayState_Pause,
    };

    VoiceInfo* _pVoiceInfo;
};
static_assert(sizeof(VoiceType) == 0x8);

struct BiquadFilterParameter {
    bool enable;
    s16 numerator[3];
    s16 denominator[2];
};
static_assert(sizeof(BiquadFilterParameter) == 0xc);
} // namespace nn::audio