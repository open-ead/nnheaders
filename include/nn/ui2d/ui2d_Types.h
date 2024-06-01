#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_ResTexture.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Texture.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util.h>
#include <nn/util/util_MathTypes.h>

namespace nn {
namespace ui2d {

class Layout;

const int ResourceNameStrMax = 24;
const int TexMapMax = 3;
const int TevStageMax = 6;
const int MatColorMax = 2;
const int TexImageNameMax = 128;
const int ShaderNameMax = 8;
const int ArchiveShaderNameMax = 6;
const int GroupNameStrMax = 32;
const int ConstantBufferCount = 2;
const int DetailedCombinerConstantColor = 5;
const int ShaderVariationCountPerBlend = 3;
const int TextBoxLineWidthOffsetCount = 16;
const int CombinerUserShaderConstantColor = 5;
const size_t CaptureTexturePathMax = 64;
const size_t CaptureTexturePartsLayerMax = 8;
const char CaptureTexturePathSeparator = '%';

static const char* ArchiveShaderFileName;
static const char* ArchiveShaderRegistrationName;
static const char* ArchiveShaderRegistrationNameWithDetailedCombiner;
static const char* ArchiveShaderRegistrationNameWithCombinerUserShader;
static const char* ArchiveShaderVariationTableFileName;
static const char* IndividualArchiveShaderVariationTableBaseName;
static const char* IndividualArchiveShaderVariationTableExtensionName;

const int IndividualArchiveShaderVariationTableFirstBlendWithDetailedCombiner = 110;
const int IndividualArchiveShaderVariationTableFirstBlendWithCombinerUserShader = 111;
const int IndividualArchiveShaderVariationTableFirstBlendWithMaskShader = 120;
const int PerspectiveTextureProjectionDefaultShader = 250;
const int PerspectiveTextureProjectionBlendShaderOffset = 200;
const int DetailedCombinerStageBitsCountWithVariationTable = 24;

static const char* pCombinerUserShaderExtUserData_Vec3[4];
static const char* pCombinerUserShaderExtUserData_Vec2[4];
static const char* pCombinerUserShaderExtUserData_Rgba[4];
static const char* pCombinerUserShaderExtUserData_IVec2[4];
static const char* pCombinerUserShaderExtUserData_Int[4];
static const char* pCombinerUserShaderExtUserData_Float[4];

namespace detail {

template <typename T>
void SetBit(T* pBits, int pos, bool val) {
    const T mask = static_cast<T>(1 << pos);

    if (val) {
        *pBits |= mask;
    } else {
        *pBits &= ~mask;
    }
}

template <typename T>
bool TestBit(T bits, int pos) {
    const T mask = static_cast<T>(1 << pos);
    return bits & mask;
}

}  // namespace detail

template <typename TToPtr, typename TFrom>
TToPtr DynamicCast(TFrom* obj) {
    return font::DynamicCast<TToPtr>(obj);
}

template <typename To, typename From>
bool IsDerivedFrom(const From* instance) {
    return font::IsDerivedFrom<To>(instance);
}

typedef bool (*RegisterTextureViewSlot)(gfx::DescriptorSlot*, const gfx::TextureView&, void*);
typedef bool (*RegisterSamplerSlot)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);
typedef void (*UnregisterTextureViewSlot)(gfx::DescriptorSlot*, const gfx::TextureView&, void*);
typedef void (*UnregisterSamplerSlot)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);

enum PaneFlag {
    PaneFlag_Visible,
    PaneFlag_InfluencedAlpha,
    PaneFlag_LocationAdjust,
    PaneFlag_UserAllocated,
    PaneFlag_IsGlobalMatrixDirty,
    PaneFlag_UserMatrix,
    PaneFlag_UserGlobalMatrix,
    PaneFlag_IsConstantBufferReady,
    PaneFlag_MaxPaneFlag
};

enum PaneFlagEx {
    PaneFlagEx_IgnorePartsMagnify,
    PaneFlagEx_PartsMagnifyAdjustToPartsBound,
    PaneFlagEx_ExtUserDataAnimationEnabled,
    PaneFlagEx_ViewerInvisible,
    PaneFlagEx_IsConstantBufferReadySelf,
    PaneFlagEx_IsCalculationFinishedSelf,
    PaneFlagEx_DynamicExtUserDataEnabled,
    PaneFlagEx_MaxPaneFlagEx
};

enum SystemDataType {
    SystemDataType_SimpleAABBMesh,
    SystemDataType_SimpleOBBMesh,
    SystemDataType_AlignmentExInfo,
    SystemDataType_MaskTexture,
    SystemDataType_CaptureTextureRuntimeCopyInfo = 16,
    SystemDataType_MaskTextureRuntimeInfo,
    SystemDataType_Max
};

enum HorizontalPosition {
    HorizontalPosition_Center,
    HorizontalPosition_Left,
    HorizontalPosition_Right,
    HorizontalPosition_MaxHorizontalPosition
};

enum VerticalPosition {
    VerticalPosition_Center,
    VerticalPosition_Top,
    VerticalPosition_Bottom,
    VerticalPosition_MaxVerticalPosition
};

enum TexWrap { TexWrap_Clamp, TexWrap_Repeat, TexWrap_Mirror, TexWrap_MaxTexWrap };

typedef gfx::ImageFormat TexFormat;

enum TexFilter { TexFilter_Near, TexFilter_Linear, TexFilter_MaxTexFilter };

enum TexProjectionFlag {
    TexProjectionFlag_FittingLayoutSize,
    TexProjectionFlag_FittingPaneSizeEnabled,
    TexProjectionFlag_AdjustProjectionScaleRotateEnabled,
    TexProjectionFlag_MaxTexProjectionFlag
};

enum TevMode {
    TevMode_Replace,
    TevMode_Modulate,
    TevMode_Add,
    TevMode_AddSigned,
    TevMode_Interpolate,
    TevMode_Subtract,
    TevMode_AddMultiply,
    TevMode_MultiplyAdd,
    TevMode_Overlay,
    TevMode_Lighten,
    TevMode_Darken,
    TevMode_Indirect,
    TevMode_BlendIndirect,
    TevMode_EachIndirect,
    TevMode_MaxTevMode
};

enum AlphaTest {
    AlphaTest_Never,
    AlphaTest_Less,
    AlphaTest_LessEqual,
    AlphaTest_Equal,
    AlphaTest_NotEqual,
    AlphaTest_GreaterEqual,
    AlphaTest_Greater,
    AlphaTest_Always,
    AlphaTest_MaxAlphaTest
};

enum BlendFactor {
    BlendFactor_0,
    BlendFactor_1,
    BlendFactor_DstColor,
    BlendFactor_InvDstColor,
    BlendFactor_SrcAlpha,
    BlendFactor_InvSrcAlpha,
    BlendFactor_DstAlpha,
    BlendFactor_InvDstAlpha,
    BlendFactor_SrcColor,
    BlendFactor_InvSrcColor,
    BlendFactor_MaxBlendFactor
};

enum BlendOp {
    BlendOp_Disable,
    BlendOp_Add,
    BlendOp_Subtract,
    BlendOp_ReverseSubtract,
    BlendOp_SelectMin,
    BlendOp_SelectMax,
    BlendOp_MaxBlendOp
};

enum LogicOp {
    LogicOp_Disable,
    LogicOp_Noop,
    LogicOp_Clear,
    LogicOp_Set,
    LogicOp_Copy,
    LogicOp_InvCopy,
    LogicOp_Inv,
    LogicOp_And,
    LogicOp_Nand,
    LogicOp_Or,
    LogicOp_Nor,
    LogicOp_Xor,
    LogicOp_Equiv,
    LogicOp_RevAnd,
    LogicOp_InvAnd,
    LogicOp_RevOr,
    LogicOp_InvOr,
    LogicOp_MaxLogicOp
};

enum TexGenType { TexGenType_Mtx2x4, TexGenType_MaxTexGenType };

enum TexGenSrc {
    TexGenSrc_Tex0,
    TexGenSrc_Tex1,
    TexGenSrc_Tex2,
    TexGenSrc_OrthoProjection,
    TexGenSrc_PaneBaseOrthoProjection,
    TexGenSrc_PerspectiveProjection,
    TexGenSrc_PaneBasePerspectiveProjection,
    TexGenSrc_MaxTexGenSrc
};

enum TextureFlip {
    TextureFlip_None,
    TextureFlip_FlipU,
    TextureFlip_FlipV,
    TextureFlip_Rotate90,
    TextureFlip_Rotate180,
    TextureFlip_Rotate270,
    TextureFlip_MaxTextureFlip
};

enum TextBoxFlag {
    TextBoxFlag_ShadowEnabled,
    TextBoxFlag_ForceAssignTextLength,
    TextBoxFlag_InvisibleBorderEnabled,
    TextBoxFlag_DoubleDrawnBorderEnabled,
    TextBoxFlag_PerCharacterTransformEnabled,
    TextBoxFlag_CenterCeilingEnabled,
    TextBoxFlag_LineWidthOffsetEnabled,
    TextBoxFlag_ExtendedTagEnabled,
    TextBoxFlag_PerCharacterTransformSplitByCharWidth,
    TextBoxFlag_PerCharacterTransformAutoShadowAlpha,
    TextBoxFlag_DrawFromRightToLeft,
    TextBoxFlag_PerCharacterTransformOriginToCenter,
    TextBoxFlag_MaxTextBoxFlag
};

enum TextBoxOverrideUsageFlag {
    TextBoxOverrideUsageFlag_TextEnabled,
    TextBoxOverrideUsageFlag_MaxTextBoxOverrideUsageFlag
};

enum BasicOverrideUsageFlag {
    BasicOverrideUsageFlag_VisibleEnabled,
    BasicOverrideUsageFlag_VisibleValue,
    BasicOverrideUsageFlag_BasicUserDataEnabled,
    BasicOverrideUsageFlag_TranslateEnabled,
    BasicOverrideUsageFlag_SizeEnabled,
    BasicOverrideUsageFlag_ScaleEnabled,
    BasicOverrideUsageFlag_RotateEnabled,
    BasicOverrideUsageFlag_AlphaEnabled,
    BasicOverrideUsageFlag_MaxBasicOverrideUsageFlag
};

enum MaterialOverrideUsageFlag {
    MaterialOverrideUsageFlag_InterpolateColorEnabled,
    MaterialOverrideUsageFlag_TextureEnabled,
    MaterialOverrideUsageFlag_OverrideCaptureTextureResources,
    MaterialOverrideUsageFlag_MaxMaterialOverrideUsageFlag
};

enum ExtUserDataOverrideOffset {
    ExtUserDataOverrideOffset_NoOverride,
    ExtUserDataOverrideOffset_NoData
};

enum TextAlignment {
    TextAlignment_Synchronous,
    TextAlignment_Left,
    TextAlignment_Center,
    TextAlignment_Right,
    TextAlignment_MaxTextAlignment
};

enum VertexColor {
    VertexColor_LeftTop,
    VertexColor_RightTop,
    VertexColor_LeftBottom,
    VertexColor_RightBottom,
    VertexColor_MaxVertexColor
};

enum WindowFlag {
    WindowFlag_UseOneMaterialForAll,
    WindowFlag_UseVertexColorAll,
    WindowFlag_WindowKind0,
    WindowFlag_WindowKind1,
    WindowFlag_NotDrawContent,
    WindowFlag_MaxWindowFlag,
    WindowFlag_WindowKindLength = 2
};

enum WindowFrame {
    WindowFrame_LeftTop,
    WindowFrame_RightTop,
    WindowFrame_LeftBottom,
    WindowFrame_RightBottom,
    WindowFrame_Left,
    WindowFrame_Right,
    WindowFrame_Top,
    WindowFrame_Bottom,
    WindowFrame_MaxWindowFrame
};

enum WindowInflationFixedPoint {
    WindowInflationFixedPoint_FractionalBit = 4,
    WindowInflationFixedPoint_IntegerBit = 11,
    WindowInflationFixedPoint_SignBit = 1,
    WindowInflationFixedPoint_ScalingFactor = 16
};

enum TextColor { TextColor_Top, TextColor_Bottom, TextColor_MaxTextColor };

enum InterpolateColor {
    InterpolateColor_Black,
    InterpolateColor_White,
    InterpolateColor_MaxInterpolateColor
};

enum PicturePaneFlag { PicturePaneFlag_ShapeBinaryIndex, PicturePaneFlag_Max };

enum ExtUserDataType {
    ExtUserDataType_String,
    ExtUserDataType_Int,
    ExtUserDataType_Float,
    ExtUserDataType_SystemData,
    ExtUserDataType_MaxExtUserDataType
};

enum AnimContentType {
    AnimContentType_Pane,
    AnimContentType_Material,
    AnimContentType_ExtUserData,
    AnimContentType_MaxAnimContentType
};

enum AnimTagFlag { AnimTagFlag_DescendingBind, AnimTagFlag_MaxAnimTagFlag };

enum AnimTargetPane {
    AnimTargetPane_TranslateX,
    AnimTargetPane_TranslateY,
    AnimTargetPane_TranslateZ,
    AnimTargetPane_RotateX,
    AnimTargetPane_RotateY,
    AnimTargetPane_RotateZ,
    AnimTargetPane_ScaleX,
    AnimTargetPane_ScaleY,
    AnimTargetPane_SizeWidth,
    AnimTargetPane_SizeHeight,
    AnimTargetPane_MaxAnimTargetPane
};

enum AnimTargetVisibility {
    AnimTargetVisibility_Visibility,
    AnimTargetVisibility_MaxAnimTargetVisibility
};

enum AnimTargetPaneColor {
    AnimTargetPaneColor_VertexLtR,
    AnimTargetPaneColor_VertexLtG,
    AnimTargetPaneColor_VertexLtB,
    AnimTargetPaneColor_VertexLtA,
    AnimTargetPaneColor_VertexRtR,
    AnimTargetPaneColor_VertexRtG,
    AnimTargetPaneColor_VertexRtB,
    AnimTargetPaneColor_VertexRtA,
    AnimTargetPaneColor_VertexLbR,
    AnimTargetPaneColor_VertexLbG,
    AnimTargetPaneColor_VertexLbB,
    AnimTargetPaneColor_VertexLbA,
    AnimTargetPaneColor_VertexRbR,
    AnimTargetPaneColor_VertexRbG,
    AnimTargetPaneColor_VertexRbB,
    AnimTargetPaneColor_VertexRbA,
    AnimTargetPaneColor_MaxVertex,
    AnimTargetPaneColor_PaneAlpha = 16,
    AnimTargetPaneColor_MaxAnimTargetPaneColor
};

enum AnimTargetMatColor {
    AnimTargetMatColor_BufferR,
    AnimTargetMatColor_BufferG,
    AnimTargetMatColor_BufferB,
    AnimTargetMatColor_BufferA,
    AnimTargetMatColor_Konst0R,
    AnimTargetMatColor_Konst0G,
    AnimTargetMatColor_Konst0B,
    AnimTargetMatColor_Konst0A,
    AnimTargetMatColor_C0_R,
    AnimTargetMatColor_C0_G,
    AnimTargetMatColor_C0_B,
    AnimTargetMatColor_C0_A,
    AnimTargetMatColor_C1_R,
    AnimTargetMatColor_C1_G,
    AnimTargetMatColor_C1_B,
    AnimTargetMatColor_C1_A,
    AnimTargetMatColor_C2_R,
    AnimTargetMatColor_C2_G,
    AnimTargetMatColor_C2_B,
    AnimTargetMatColor_C2_A,
    AnimTargetMatColor_C3_R,
    AnimTargetMatColor_C3_G,
    AnimTargetMatColor_C3_B,
    AnimTargetMatColor_C3_A,
    AnimTargetMatColor_C4_R,
    AnimTargetMatColor_C4_G,
    AnimTargetMatColor_C4_B,
    AnimTargetMatColor_C4_A,
    AnimTargetMatColor_MaxAnimTargetMatColor
};

enum AnimTargetMatColorFloat {
    AnimTargetMatColorFloat_BufferR = 28,
    AnimTargetMatColorFloat_BufferG,
    AnimTargetMatColorFloat_BufferB,
    AnimTargetMatColorFloat_BufferA,
    AnimTargetMatColorFloat_Konst0R,
    AnimTargetMatColorFloat_Konst0G,
    AnimTargetMatColorFloat_Konst0B,
    AnimTargetMatColorFloat_Konst0A,
    AnimTargetMatColorFloat_C0_R,
    AnimTargetMatColorFloat_C0_G,
    AnimTargetMatColorFloat_C0_B,
    AnimTargetMatColorFloat_C0_A,
    AnimTargetMatColorFloat_C1_R,
    AnimTargetMatColorFloat_C1_G,
    AnimTargetMatColorFloat_C1_B,
    AnimTargetMatColorFloat_C1_A,
    AnimTargetMatColorFloat_C2_R,
    AnimTargetMatColorFloat_C2_G,
    AnimTargetMatColorFloat_C2_B,
    AnimTargetMatColorFloat_C2_A,
    AnimTargetMatColorFloat_C3_R,
    AnimTargetMatColorFloat_C3_G,
    AnimTargetMatColorFloat_C3_B,
    AnimTargetMatColorFloat_C3_A,
    AnimTargetMatColorFloat_C4_R,
    AnimTargetMatColorFloat_C4_G,
    AnimTargetMatColorFloat_C4_B,
    AnimTargetMatColorFloat_C4_A,
    AnimTargetMatColorFloat_MaxAnimTargetMatColor
};

enum AnimTargetTexSrt {
    AnimTargetTexSrt_TranslateS,
    AnimTargetTexSrt_TranslateT,
    AnimTargetTexSrt_Rotate,
    AnimTargetTexSrt_ScaleS,
    AnimTargetTexSrt_ScaleT,
    AnimTargetTexSrt_MaxAnimTargetTexSrt
};

enum AnimTargetTexPattern {
    AnimTargetTexPattern_Image,
    AnimTargetTexPattern_MaxAnimTargetTexPattern
};

enum AnimTargetIndirectSrt {
    AnimTargetIndirectSrt_Rotate,
    AnimTargetIndirectSrt_ScaleS,
    AnimTargetIndirectSrt_ScaleT,
    AnimTargetIndirectSrt_MaxAnimTargetIndirectSrt
};

enum AnimTargetFontShadow {
    AnimTargetFontShadow_BlackInterporateColorR,
    AnimTargetFontShadow_BlackInterporateColorG,
    AnimTargetFontShadow_BlackInterporateColorB,
    AnimTargetFontShadow_WhiteInterporateColorR,
    AnimTargetFontShadow_WhiteInterporateColorG,
    AnimTargetFontShadow_WhiteInterporateColorB,
    AnimTargetFontShadow_WhiteInterporateColorA,
    AnimTargetFontShadow_MaxAnimTargetFontShadow
};

enum AnimTargetPerCharacterTransform {
    AnimTargetPerCharacterTransform_EvalTypeOffset,
    AnimTargetPerCharacterTransform_EvalTypeWidth,
    AnimTargetPerCharacterTransform_MaxAnimTargetPerCharacterTransform
};

enum AnimTargetPerCharacterTransformCurve {
    AnimTargetPerCharacterTransformCurve_TranslateX,
    AnimTargetPerCharacterTransformCurve_TranslateY,
    AnimTargetPerCharacterTransformCurve_TranslateZ,
    AnimTargetPerCharacterTransformCurve_RotateX,
    AnimTargetPerCharacterTransformCurve_RotateY,
    AnimTargetPerCharacterTransformCurve_RotateZ,
    AnimTargetPerCharacterTransformCurve_LeftTopR,
    AnimTargetPerCharacterTransformCurve_LeftTopG,
    AnimTargetPerCharacterTransformCurve_LeftTopB,
    AnimTargetPerCharacterTransformCurve_LeftTopA,
    AnimTargetPerCharacterTransformCurve_LeftBottomR,
    AnimTargetPerCharacterTransformCurve_LeftBottomG,
    AnimTargetPerCharacterTransformCurve_LeftBottomB,
    AnimTargetPerCharacterTransformCurve_LeftBottomA,
    AnimTargetPerCharacterTransformCurve_ScaleX,
    AnimTargetPerCharacterTransformCurve_ScaleY,
    AnimTargetPerCharacterTransformCurve_MaxAnimTargetPerCharacterTransformCurve
};

enum AnimTargetWindow {
    AnimTargetWindow_FrameTop,
    AnimTargetWindow_FrameBottom,
    AnimTargetWindow_FrameLeft,
    AnimTargetWindow_FrameRight,
    AnimTargetWindow_MaxAnimTargetWindow
};

enum PerCharacterTransformLoopType {
    PerCharacterTransformLoopType_OneTime,
    PerCharacterTransformLoopType_Loop,
    PerCharacterTransformLoopType_MaxPerCharacterTransformLoopType
};

enum AnimTargetAlphaCompare {
    AnimTargetAlphaCompare_Ref,
    AnimTargetAlphaCompare_MaxAnimTargetAlphaCompare
};

enum AnimTargetMaskTexMtxSrt {
    AnimTargetMaskTexSrt_TranslateX,
    AnimTargetMaskTexSrt_TranslateY,
    AnimTargetMaskTexSrt_Rotate,
    AnimTargetMaskTexSrt_ScaleX,
    AnimTargetMaskTexSrt_ScaleY,
    AnimTargetMaskTexSrt_MaxAnimTargetMask
};

enum AnimCurve {
    AnimCurve_Constant,
    AnimCurve_Step,
    AnimCurve_Hermite,
    AnimCurve_ParameterizedAnim,
    AnimCurve_MaxAnimCurve
};

enum ShaderVariation {
    ShaderVariation_Standard,
    ShaderVariation_WithoutVertexColor,
    ShaderVariation_GfxPrimitive,
    ShaderVariation_MaxShaderVariation
};

enum RenderTargetTextureLifetime {
    RenderTargetTextureLifetime_Layout,
    RenderTargetTextureLifetime_OneFrame,
    RenderTargetTextureLifetime_Max,
    RenderTargetTextureLifetime_Invalid = -1
};

struct WindowFrameSize {
    float left;
    float right;
    float top;
    float bottom;
};

struct Size {
    static Size Create(float, float);

    void Set(float aWidth, float aHeight) {
        width = aWidth;
        height = aHeight;
    }

    float width;
    float height;
};

class TexSize {
public:
    TexSize() : width(0), height(0) {}
    TexSize(uint16_t aWidth, uint16_t aHeight) : width(aWidth), height(aHeight) {}
    TexSize(const TexSize& other) : width(other.width), height(other.height) {}

    uint16_t width;
    uint16_t height;
};

class TextureInfo {
    NN_NO_COPY(TextureInfo);

public:
    NN_RUNTIME_TYPEINFO_BASE();

    static const uint64_t InvalidSlot = 0xFFFFFFFFFFFFFFFF;

    TextureInfo() { ResetTextureDescriptorSlot(); }

    virtual ~TextureInfo() = default;
    virtual void Finalize(gfx::Device*) = 0;

    void ResetTextureDescriptorSlot() { m_DescriptorSlot.ToData()->value = InvalidSlot; }

    void Set(const TextureInfo& value);
    virtual const TexSize GetSize() const = 0;

    virtual TexFormat GetFormat() const { return gfx::ImageFormat_Undefined; }

    virtual bool IsValid() const = 0;

    bool IsTextureDescriptorSlotReady() const {
        return m_DescriptorSlot.ToData()->value != InvalidSlot;
    }

    virtual const gfx::TextureView* GetTextureView() const = 0;
    virtual gfx::TextureView* GetTextureView() = 0;

    const gfx::DescriptorSlot* GetTextureDescriptorSlot() const { return &m_DescriptorSlot; }
    gfx::DescriptorSlot* GetTextureDescriptorSlot() { return &m_DescriptorSlot; }

private:
    static bool IsPowerOfTwo(int);

    gfx::DescriptorSlot m_DescriptorSlot;
};

class PlacementTextureInfo : public TextureInfo {
    NN_NO_COPY(PlacementTextureInfo);

public:
    NN_RUNTIME_TYPEINFO(TextureInfo);

    PlacementTextureInfo() {}

    virtual ~PlacementTextureInfo() = default;
    virtual void Finalize(gfx::Device* pDevice) { NN_UNUSED(pDevice); }

    void SetSize(int width, int height) {
        m_Size.width = width;
        m_Size.height = height;
    }

    virtual const TexSize GetSize() const { return m_Size; }

    virtual bool IsValid() const { return IsTextureDescriptorSlotReady(); }

    virtual const gfx::TextureView* GetTextureView() const { return nullptr; }
    virtual gfx::TextureView* GetTextureView() { return nullptr; }

private:
    TexSize m_Size;
};

class ResourceTextureInfo : public TextureInfo {
    NN_NO_COPY(ResourceTextureInfo);

public:
    NN_RUNTIME_TYPEINFO(TextureInfo);

    ResourceTextureInfo() : m_pResTexture(nullptr) {}

    virtual ~ResourceTextureInfo() = default;

    virtual bool IsValid() const {
        const TexSize size = GetSize();
        return size.width != 0 && size.height != 0;
    }

    virtual const TexSize GetSize() const {
        gfx::TextureInfo* pTextureInfo = m_pResTexture->GetTextureInfo();
        return TexSize(pTextureInfo->GetWidth(), pTextureInfo->GetHeight());
    }

    const gfx::Texture* GetTexture() const { return m_pResTexture->GetTexture(); }
    gfx::Texture* GetTexture() { return m_pResTexture->GetTexture(); }

    virtual const gfx::TextureView* GetTextureView() const {
        return m_pResTexture->GetTextureView();
    }

    virtual gfx::TextureView* GetTextureView() { return m_pResTexture->GetTextureView(); }

    const gfx::ResTexture* GetResTexture() const { return m_pResTexture; }

    virtual TexFormat GetFormat() const {
        return m_pResTexture->GetTextureInfo()->GetImageFormat();
    }

    void InitializeFromResource(gfx::Device* pDevice, gfx::ResTexture* pResTexture);

    virtual void Finalize(gfx::Device* pDevice) {
        if (m_pResTexture) {
            gfx::Texture* pTexture = m_pResTexture->GetTexture();
            if (pTexture && gfx::IsInitialized(*pTexture)) {
                m_pResTexture->Finalize(pDevice);
                m_pResTexture = nullptr;
            }
        }
    }

private:
    gfx::ResTexture* m_pResTexture;
};

class RenderTargetTextureInfo : public PlacementTextureInfo {
    NN_NO_COPY(RenderTargetTextureInfo);

public:
    NN_RUNTIME_TYPEINFO(PlacementTextureInfo);

    RenderTargetTextureInfo();

    virtual ~RenderTargetTextureInfo();
    virtual bool IsValid() const;
    const gfx::Texture* GetTexture() const;
    gfx::Texture* GetTexture();
    virtual const gfx::TextureView* GetTextureView() const;
    virtual gfx::TextureView* GetTextureView();
    gfx::ColorTargetView* GetColorTargetView();
    const gfx::ColorTargetView* GetColorTargetView() const;
    gfx::ViewportScissorState* GetViewportScissorState();
    const gfx::ViewportScissorState* GetViewportScissorState() const;
    virtual TexFormat GetFormat() const;
    RenderTargetTextureLifetime GetLifetime() const;
    void Initialize(gfx::Device*, const gfx::TextureInfo&, RenderTargetTextureLifetime);
    void Initialize(gfx::Device*, const Layout*, const gfx::TextureInfo&,
                    RenderTargetTextureLifetime);
    virtual void Finalize(gfx::Device*);

private:
    gfx::Texture* m_pTexture;
    gfx::TextureView* m_pTextureView;
    gfx::DescriptorSlot* m_pTextureSlot;
    const Layout* m_pOwnerLayout;
    gfx::ColorTargetView m_ColorTargetView;
    gfx::ViewportScissorState m_ViewportScissorState;
    TexFormat m_Format;
    RenderTargetTextureLifetime m_LifeTime;
};

typedef util::Float2 TexCoordQuad[4];
typedef const void* FontKey;
typedef uint32_t ResType;

enum HorizontalAlignment {
    HorizontalAlignment_Left,
    HorizontalAlignment_Center,
    HorizontalAlignment_Right,
    HorizontalAlignment_Max
};

}  // namespace ui2d
}  // namespace nn
