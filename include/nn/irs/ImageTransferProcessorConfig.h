#pragma once

#include <nn/irs/ImageTransferProcessorFormat.h>
#include <nn/types.h>

namespace nn::irsensor {
struct __attribute__((packed)) ImageTransferProcessorConfig {
    s64 exposureTime = 300000;
    s32 lightTarget = 0;
    s32 gain = 8;
    bool isNegativeImageUsed = false;
    u8 reserved[7];
    ImageTransferProcessorFormat format = ImageTransferProcessorFormat::_320x240;
};
}  // namespace nn::irsensor
