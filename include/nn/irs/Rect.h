#pragma once

#include <nn/types.h>

namespace nn::irsensor {
struct Rect {
    u16 x = 0;
    u16 y = 0;
    u16 width = 320;
    u16 height = 240;
};
}  // namespace nn::irsensor
