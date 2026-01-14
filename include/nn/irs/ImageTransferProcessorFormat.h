#pragma once

#include <nn/types.h>

namespace nn::irsensor {
enum class ImageTransferProcessorFormat : u32 {
    _320x240,
    _160x120,
    _80x60,
    _40x30,  // [4.0.0+]
    _20x15,  // [4.0.0+]
};
}  // namespace nn::irsensor
