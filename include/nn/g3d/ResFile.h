/**
 * @file ResFile.h
 * @brief Resource file for models.
 */

#pragma once

#include <nn/types.h>

#include <nn/gfx/gfx_BufferInfo.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>
#include <nn/gfx/gfx_Types.h>

#include <nn/util.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_ResDic.h>

namespace nn {

namespace gfx {
template <typename T>
class TDevice;
}

namespace g3d {
class ResModel;
class ResMaterialAnim;
class ResShapeAnim;
class ResSceneAnim;
class ResSkeletalAnim;
class ResBoneVisibilityAnim;
struct TextureRef;

using TextureBindCallback = nn::g3d::TextureRef (*)(char const*, void*);

struct ResFileData {
    util::BinaryFileHeader fileHeader;
    util::BinTPtr<util::BinString> pFileName;
    util::BinTPtr<ResModel> pModels;
    util::BinTPtr<util::ResDic> pModelDict;
    util::BinTPtr<ResSkeletalAnim> pSkeletalAnims;
    util::BinTPtr<util::ResDic> pSkeletalAnimDict;
    util::BinTPtr<ResMaterialAnim> pMaterialAnims;
    util::BinTPtr<util::ResDic> pMaterialAnimsDict;
    util::BinTPtr<ResBoneVisibilityAnim> pBoneVisibilityAnims;
    util::BinTPtr<util::ResDic> pBoneVisiDict;
    util::BinTPtr<ResShapeAnim> pShapeAnims;
    util::BinTPtr<util::ResDic> pShapeAnimDict;
    util::BinTPtr<ResSceneAnim> pSceneAnims;
    util::BinTPtr<util::ResDic> pSceneAnimDict;
    util::BinTPtr<gfx::MemoryPool> pMemoryPool;
    util::BinTPtr<gfx::MemoryPoolInfo> pMemoryPoolInfo;
    u64 embeddedFilesOffset;
    util::BinTPtr<util::ResDic> pEmbeddedFilesDict;
    u64 padding;
    u64 strTableOffset;
    u32 strTableSize;
    u16 modelCount;
    u16 skeletalAnimCount;
    u16 materialAnimCount;
    u16 boneAnimCount;
    u16 shapeAnimCount;
    u16 sceneAnimCount;
    u16 externalFileCount;
};

class ResFile : public nn::util::AccessorBase<ResFileData> {
public:
    static constexpr s64 Signature = 0x2020202053455246;  // "FRES    "

    static bool IsValid(const void* modelSrc);

    void Relocate();
    void Unrelocate();

    static nn::g3d::ResFile* ResCast(void*);

    s32 BindTexture(TextureBindCallback callback, void*);
    void ReleaseTexture();

    void Setup(gfx::Device*);
    void Setup(gfx::Device*, gfx::MemoryPool*, s64, u64);

    void Cleanup(gfx::Device*);
    void Reset();
};
static_assert(sizeof(ResFile) == 0xd0);
}  // namespace g3d
}  // namespace nn
