#pragma once

#include <nn/audio/audio_Common.h>

namespace nn::audio {
struct FinalMixType {
    MixInfo* _pMixInfo;

    static float GetVolumeMax();
    static float GetVolumeMin();
};
} // namespace nn::audio