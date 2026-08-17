#include <nn/atk/atk_Adpcm.h>

namespace nn::atk::detail {
void DecodeDspAdpcm(position_t playPosition, AdpcmContext& context, 
                    const AdpcmParam& param, const void* adpcmData, 
                    size_t decodeSamples, s16* dest) {
    if (decodeSamples != 0) {
        // TODO
    }
}
} // namespace nn::atk::detail