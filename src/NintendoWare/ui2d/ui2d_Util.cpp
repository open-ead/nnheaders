#include <nn/ui2d/ui2d_Util.h>

namespace nn {
namespace ui2d {

/*
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

int ComplexFontHelper::SetupTextureCacheArg(font::TextureCache::InitializeArg*, AcquireFontFunction,
                                            void*, const void*);
font::Font* ComplexFontHelper::InitializeComplexFontTree(gfx::Device*,
                                                         font::RegisterTextureViewSlot, void*,
                                                         font::TextureCache*, int,
                                                         AcquireFontFunction, void*, const void*);
void ComplexFontHelper::FinalizeComplexFontTree(gfx::Device*, font::Font*,
                                                font::UnregisterTextureViewSlot, void*);
bool ComplexFontHelper::CheckExt(const char*, const char*);

template <typename _1, typename _2>
void ComplexFontHelper::BuildTextureCacheArg(const void*, int*, font::TextureCache::InitializeArg*,
                                             AcquireFontFunction, void*, unsigned int);

template <typename _1>
font::Font* ComplexFontHelper::BuildFontTree(gfx::Device*, font::RegisterTextureViewSlot, void*,
                                             const void*, int*, font::TextureCache*,
                                             AcquireFontFunction, void*, unsigned int);

void ComplexFontHelper::DestroyFontTree(gfx::Device*, font::Font*, font::UnregisterTextureViewSlot,
                                        void*);
*/

/* smo
void RoundRectShape::RoundRectShape(gfx::util::PrimitiveShapeFormat, gfx::PrimitiveTopology, float,
                                    float, unsigned int);
void RoundRectShape::RoundRectShape(gfx::util::PrimitiveShapeFormat, gfx::PrimitiveTopology);
void RoundRectShape::~RoundRectShape();
void RoundRectShape::CopyParams(const RoundRectShape&);
int RoundRectShape::CalculateVertexCount();
int RoundRectShape::CalculateIndexCount();
void* RoundRectShape::CalculateVertexBuffer();
void RoundRectShape::CalculateIndexBuffer<unsigned char>();
void RoundRectShape::CalculateIndexBuffer<unsigned int>();
void RoundRectShape::CalculateIndexBuffer<unsigned short>();
void RoundRectShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void Ui2dCircleShape::CopyParams(const Ui2dCircleShape&);
*/

}  // namespace ui2d
}  // namespace nn
