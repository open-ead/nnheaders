#pragma once

#include <nn/audio/audio_Adpcm.h>

#include <nn/atk/detail/atk_Config.h>

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

namespace detail {
void DecodeDspAdpcm(position_t playPosition, AdpcmContext& context, 
                    const AdpcmParam& param, const void* adpcmData, 
                    size_t decodeSamples, s16* dest);
};
} // namespace nn::atk