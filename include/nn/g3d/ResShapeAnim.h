/**
 * @file ResShapeAnim.h
 * @brief Resource file for shape animations.
 */

#pragma once

#include <nn/types.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn {
namespace g3d {
class ResShapeAnim : util::BinaryBlockHeader {
public:
    void Reset();

    u8 _0[0x50];
};
}  // namespace g3d
}  // namespace nn
