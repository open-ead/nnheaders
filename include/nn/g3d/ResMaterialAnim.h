/**
 * @file ResMaterialAnim.h
 * @brief Resource file for material animations.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace g3d {
struct TextureRef;

class ResMaterialAnim {
public:
    void ReleaseTexture();
    s32 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void Reset();

    u8 _0[0x78];
};
}  // namespace g3d
}  // namespace nn
