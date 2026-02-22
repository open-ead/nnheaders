#pragma once

#include <nn/audio/audio_Adpcm.h>

namespace nn::atk {
using AdpcmParam = audio::AdpcmParameter;

struct alignas(64) AdpcmContext {
    audio::AdpcmContext audioAdpcmContext;
};
static_assert(sizeof(AdpcmContext) == 0x40);

struct AdpcmContextNotAligned {
    audio::AdpcmContext audioAdpcmContext;
};
static_assert(sizeof(AdpcmContextNotAligned) == 0x6);
} // namespace nn::atk