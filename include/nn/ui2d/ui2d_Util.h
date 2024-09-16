#pragma once

#include <nn/font/font_TextureCache.h>
#include <nn/font/font_Types.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/util/util_MathTypes.h>

namespace nn {

namespace font {
class Font;
}

namespace gfx {
class ResShaderContainer;
class ResShaderProgram;
class ResShaderVariation;
}  // namespace gfx

namespace ui2d {

class AnimTransform;
class DrawInfo;
class Group;
class Layout;
class Pane;
class Material;
class ResourceAccessor;
struct ResHermiteKey;
struct ResParameterizedAnim;
class ResParameterizedAnimParameter;
class ResourceTextureInfo;
class ShaderInfo;
struct ResExtUserDataList;
class ResExtUserData;
struct BuildArgSet;

bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const void*);
void LoadArchiveShader(ShaderInfo*, gfx::Device*, void*, const void*, gfx::MemoryPool*, long,
                       unsigned long);
void FreeArchiveShader(gfx::Device*, ShaderInfo*);
void ConvertBlendsToArchiveShaderName(char*, int, int);
void ConvertStageBitsToArchiveShaderDetailedCombinerName(char*, int, int, int, int*);
bool ConvertArchiveShaderNameToBlends(int*, int*, const char*);
void ConvertArchiveShaderDetailedCombinerNameToStageBits(int*, const char*);
int SearchShaderVariationIndexFromTable(const void*, int, int);
int SearchShaderVariationDetailedCombinerIndexFromTable(const void*, int*);
void BindAnimation(AnimTransform*, Group*, bool);
void UnbindAnimation(AnimTransform*, Group*);
bool IsContain(const Pane*, const util::Float2&);
Pane* FindHitPane(Pane*, const util::Float2&);
const Pane* FindHitPane(const Pane*, const util::Float2&);
Pane* FindHitPane(Layout*, const util::Float2&);
const Pane* FindHitPane(const Layout*, const util::Float2&);
Pane* GetNextPane(Pane*);
Pane* ClonePaneTree(const Pane*, gfx::Device*);
Pane* ClonePaneTree(const Pane*, gfx::Device*, ResourceAccessor*, const char*);
Pane* ClonePaneTreeWithPartsLayout(const Pane*, Layout*, gfx::Device*, Layout*);
Pane* ClonePaneTreeWithPartsLayout(const Pane*, Layout*, gfx::Device*, Layout*, ResourceAccessor*,
                                   const char*);
bool ComparePaneTreeTest(const Pane*, const Pane*);
void DrawNullAndBoundingPane(gfx::CommandBuffer&, DrawInfo&, Material&, const Pane*,
                             const util::Unorm8x4&, const util::Unorm8x4&);

float GetHermiteCurveValue(float, const ResHermiteKey*, int);

float GetParameterizedAnimValue(float, float, const ResParameterizedAnim*);
float GetParameterizedAnimValueAtFrame(float, const ResParameterizedAnimParameter*);
const ResExtUserData* GetExtUserData(const ResExtUserDataList*, const char*);
size_t GetAlignedBufferSize(gfx::Device*, gfx::GpuAccess, unsigned long);
void SetDefaultShaderId(Material*, int);
bool IsResShaderContainerInitialized(gfx::ResShaderContainer*);
bool IsResShaderProgramInitialized(gfx::ResShaderProgram*);
void MakeCaptureTextureName(char*, unsigned long, const char*, const char*);
size_t CalcCaptureTexturePrefixLength(const BuildArgSet&, int);
void ConcatCaptureTexturePrefixString(char*, unsigned long, const BuildArgSet&, int);
const TextureInfo* AcquireCaptureTextureWithResolvePrefix(char*, int, const BuildArgSet&, bool,
                                                          gfx::Device*, ResourceAccessor*,
                                                          const char*);
const TextureInfo* AcquireCaptureTexture(char*, int, gfx::Device*, ResourceAccessor*, const char*,
                                         const char*);
gfx::ShaderCodeType TryInitializeAndGetShaderCodeType(gfx::Device*, gfx::ResShaderVariation*);

class ComplexFontHelper {
    typedef void* (*AcquireFontFunction)(size_t*, const char*, ResType, void*);

public:
    static int SetupTextureCacheArg(font::TextureCache::InitializeArg*, AcquireFontFunction, void*,
                                    const void*);
    static font::Font* InitializeComplexFontTree(gfx::Device*, font::RegisterTextureViewSlot, void*,
                                                 font::TextureCache*, int, AcquireFontFunction,
                                                 void*, const void*);
    static void FinalizeComplexFontTree(gfx::Device*, font::Font*, font::UnregisterTextureViewSlot,
                                        void*);

private:
    static bool CheckExt(const char*, const char*);
    static void DestroyFontTree(gfx::Device*, font::Font*, font::UnregisterTextureViewSlot, void*);

    template <typename _1, typename _2>
    static void BuildTextureCacheArg(const void*, int*, font::TextureCache::InitializeArg*,
                                     AcquireFontFunction, void*, uint32_t);

    template <typename _1>
    static font::Font* BuildFontTree(gfx::Device*, font::RegisterTextureViewSlot, void*,
                                     const void*, int*, font::TextureCache*, AcquireFontFunction,
                                     void*, uint32_t);
};

}  // namespace ui2d
}  // namespace nn
