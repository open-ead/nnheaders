/**
 * @file ResSceneAnim.h
 * @brief Resource file for scene animations.
 */

#pragma once

#include <nn/types.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn {
namespace g3d {
class ResLightAnim;
class ResFogAnim;
class BindFuncTable;

class ResSceneAnim : public util::BinaryBlockHeader {
public:
    s32 Bind(nn::g3d::BindFuncTable const&);
    void Release();
    void Reset();

    u64 mNameOffset;
    u64 mPathOffset;
    u64 mCameraAnimOffset;
    u64 mCameraAnimDictOffset;
    nn::g3d::ResLightAnim* mLightAnims;
    u64 mLightAnimDictOffset;
    nn::g3d::ResFogAnim* mFogAnims;
    u64 mFogAnimDictOffset;
    u64 mUserDataOffset;
    u64 mUserDataDictOffset;
    u16 mUserDataCount;
    u16 mCameraAnimCount;
    u16 mLightAnimCount;
    u16 mFogAnimCount;
};
}  // namespace g3d
}  // namespace nn
