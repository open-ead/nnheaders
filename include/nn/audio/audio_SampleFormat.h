#pragma once

namespace nn::audio {
enum SampleFormat {
    SampleFormat_Invalid,
    SampleFormat_PcmInt8,
    SampleFormat_PcmInt16,
    SampleFormat_PcmInt24,
    SampleFormat_PcmInt32,
    SampleFormat_PcmFloat,
    SampleFormat_Adpcm,
};
} // namespace nn::audio