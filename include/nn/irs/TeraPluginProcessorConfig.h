#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct TeraPluginProcessorConfig {
    s8 mode;
    s8 _1;  // [6.0.0+]
    s8 _2;  // [6.0.0+]
    s8 _3;  // [6.0.0+]
};
}  // namespace nn::irsensor
