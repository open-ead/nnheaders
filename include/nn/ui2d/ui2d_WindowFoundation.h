#pragma once

#include <nn/ui2d/ui2d_Common.h>
#include <nn/util/util_MathTypes.h>

namespace nn {

namespace ui2d {

struct Size;

enum WindowFrameMode {
    WindowFrameMode_Around,
    WindowFrameMode_Horizontal,
    WindowFrameMode_HorizontalNoContent,
    WindowFrameMode_MaxWindowFrameMode
};

struct WindowContent {
    util::Unorm8x4 vtxColors[4];
    detail::TexCoordArray texCoordArray;
};

struct WindowFrameSizeInternal {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
};

struct WindowSize {
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
    WindowFrameSizeInternal frameSize;
};

namespace detail {
struct Rect {
    float x;
    float y;
    float w;
    float h;

    const util::Float2 Position() const;
    const Size Size() const;
};

}  // namespace detail
}  // namespace ui2d
}  // namespace nn
