#include <nn/atk/atk_Adpcm.h>

namespace nn::atk::detail {
void DecodeDspAdpcm(position_t playPosition, 
                    AdpcmContext& context,
                    const AdpcmParam& param, 
                    const void* adpcmData, 
                    size_t decodeSamples, 
                    s16* dest) 
{
    position_t frame = playPosition / 14;
    position_t frameFrac = playPosition - frame * 14;
    const u8* frameBegin = reinterpret_cast<const u8*>(adpcmData) + (frame * 8);

    s32 pred  = context.audioAdpcmContext.predScale >> 4;
    s32 scale = context.audioAdpcmContext.predScale & 0xF;

    for (u32 i {0}; i < decodeSamples; ++i) {
        if (frameFrac == 0) {
            const u8 pred_scale = *frameBegin;
            context.audioAdpcmContext.predScale = pred_scale;
            pred = pred_scale >> 4;
            scale = pred_scale & 0xF;
        }

        u8 code = frameBegin[frameFrac / 2 + 1];
        if (frameFrac & 1)
            code &= 0xF;
        else
            code >>= 4;

        s16 nibble = code;
        nibble <<= 12;
        nibble >>= 1;

        s16 a1 = static_cast<s16>(param.coefficients[pred * 2 + 0]);
        s16 a2 = static_cast<s16>(param.coefficients[pred * 2 + 1]);
        s16 gain = static_cast<s16>(1 << scale);

        s32 val = a1 * context.audioAdpcmContext.history[0];
        val  += a2 * context.audioAdpcmContext.history[1];
        val  += gain * nibble;
        val >>= 10;
        val  += 1;
        val >>= 1;

        if (val > SHRT_MAX)
            val = SHRT_MAX;
        else if (val < SHRT_MIN)
            val = SHRT_MIN;

        s16 smp = static_cast<s16>(val);

        context.audioAdpcmContext.history[1] = context.audioAdpcmContext.history[0];
        context.audioAdpcmContext.history[0] = smp;

        *dest = smp;
        ++dest;

        ++frameFrac;
        if (frameFrac == 14) {
            frameBegin += 8;
            frameFrac = 0;
        }
    }
}
} // namespace nn::atk::detail