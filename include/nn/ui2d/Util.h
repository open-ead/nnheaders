#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/util/MathTypes.h>

namespace nn::gfx {
class MemoryPool;
}  // namespace nn::gfx

namespace nn::ui2d {

class AnimTransform;
class DrawInfo;
class Group;
class Layout;
class Pane;
class Material;
struct ResHermiteKey;
class ResourceTextureInfo;
class ShaderInfo;
struct ResExtUserDataList;
class ResExtUserData;

bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const void*);
void LoadArchiveShader(ShaderInfo*, gfx::Device*, void*, const void*, gfx::MemoryPool*, s64, u64);
void FreeArchiveShader(gfx::Device*, ShaderInfo*);
void ConvertBlendsToArchiveShaderName(char*, s32, s32);
bool ConvertArchiveShaderNameToBlends(s32*, s32*, const char*);
s16 SearchShaderVariationIndexFromTable(const void*, s32, s32);
void BindAnimation(AnimTransform*, Group*, bool);
void UnbindAnimation(AnimTransform*, Group*);
bool IsContain(const Pane*, const util::Float2&);
Pane* FindHitPane(Pane*, const util::Float2&);
const Pane* FindHitPane(const Pane*, const util::Float2&);
Pane* FindHitPane(Layout*, const util::Float2&);
const Pane* FindHitPane(const Layout*, const util::Float2&);
Pane* GetNextPane(Pane*);
Pane* ClonePaneTree(const Pane*, gfx::Device*);
Pane* ClonePaneTreeWithPartsLayout(const Pane*, Layout*, gfx::Device*, Layout*);
bool ComparePaneTreeTest(const Pane*, const Pane*);
void DrawNullAndBoundingPane(gfx::CommandBuffer&, DrawInfo&, Material&, const Pane*,
                             const util::Unorm8x4&, const util::Unorm8x4&);
f32 GetHermiteCurveValue(float, const ResHermiteKey*, s32);
const ResExtUserData* GetExtUserData(const ResExtUserDataList*, const char*);
u64 GetAlignedBufferSize(gfx::Device*, gfx::GpuAccess, u64);
void SetDefaultShaderId(Material*, s32);

}  // namespace nn::ui2d
