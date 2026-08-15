#include <nn/atk/atk_WaveFileReader.h>

namespace nn::atk::detail {
SampleFormat WaveFileReader::GetSampleFormat(u8 format) {
    switch (format) {
    case 0:
        return SampleFormat_PcmS8;
    case 1:
        return SampleFormat_PcmS16;
    case 2:
        return SampleFormat_DspAdpcm;
    default:
        return SampleFormat_DspAdpcm;
    }
}
} // namespace nn::atk::detail