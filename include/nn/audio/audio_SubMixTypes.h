#pragma once

#include <nn/audio/audio_Common.h>

namespace nn::audio {
struct SubMixType {
    MixInfo* _pMixInfo;

    static float GetVolumeMin();
    static float GetVolumeMax();
};
} // namespace nn::audio