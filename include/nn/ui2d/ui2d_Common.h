/**
 * @file TexCoordArray.h
 * @brief Texture coordinate array implementation.
 */

#pragma once

#include <nn/types.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn {
namespace ui2d {

class Layout;
struct ResExtUserDataList;
struct ResTextureList;
struct ResFontList;
struct ResMaterialList;
struct ResShapeInfoList;
struct ResCaptureTextureList;
class ResourceAccessor;
class ControlCreator;
class TextSearcher;
struct ResPartsPaneBasicInfo;
class Pane;

struct UserShaderInformation {
    char userShaderName[6];
    uint32_t vertexShaderConstantBufferExtendSize;
    uint32_t geometryShaderConstantBufferExtendSize;
    uint32_t pixelShaderConstantBufferExtendSize;

    void SetDefault();
    bool SetShaderName(const char*);
};

typedef bool (*GetUserShaderInformationFromUserData)(UserShaderInformation&,
                                                     const ResExtUserDataList*, void*);

struct BuildResSet {
    const ResTextureList* pTextureList;
    const ResFontList* pFontList;
    const ResMaterialList* pMaterialList;
    // smo
    // const ResShapeInfoList* pShapeInfoList;

    // newer
    // const ResCaptureTextureList* pCaptureTextureList;

    ResourceAccessor* pResAccessor;
    Layout* pLayout;

    // smo
    // bool isFlytOlderThanBinary8200;
    // newer
    // bool isDetailedCombinerWithVariationVariable;
};

typedef RenderTargetTextureLifetime (*CreateRenderTargetTextureCallback)(
    gfx::Texture**, gfx::TextureView**, gfx::DescriptorSlot**, const gfx::TextureInfo&, void*,
    RenderTargetTextureLifetime);

typedef void (*DestroyRenderTargetTextureCallback)(gfx::Texture*, gfx::TextureView*,
                                                   gfx::DescriptorSlot*, void*,
                                                   RenderTargetTextureLifetime);

typedef RenderTargetTextureLifetime (*CreateRenderTargetTextureResourceCallback)(
    gfx::Texture**, gfx::TextureView**, gfx::DescriptorSlot**, const Layout*,
    const gfx::TextureInfo&, void*, RenderTargetTextureLifetime);

typedef void (*DestroyRenderTargetTextureResourceCallback)(gfx::Texture*, gfx::TextureView*,
                                                           gfx::DescriptorSlot*, const Layout*,
                                                           void*, RenderTargetTextureLifetime);

struct BuildArgSet {
    util::Float2 magnify;
    util::Float2 partsSize;
    ControlCreator* pControlCreator;
    TextSearcher* pTextSearcher;
    Layout* pLayout;
    Layout* pBodyLayout;
    const BuildResSet* pCurrentBuildResSet;
    const BuildResSet* pOverrideBuildResSet;
    uint16_t overrideUsageFlag;
    uint16_t overrideBasicUsageFlag;
    uint16_t overrideMaterialUsageFlag;
    const ResPartsPaneBasicInfo* pOverridePartsPaneBasicInfo;
    /* newer
    const char* pCaptureTexturePrefixStack[8];
    int captureTexturePrefixStackPosition;
    int captureTextureOverridePosition;
    */
    Pane* pParentPane;
    GetUserShaderInformationFromUserData pGetUserShaderInformationFromUserDataCallback;
    // smo
    // void* pGetUserShaderInformationFromUserDataCallbackUserData;
    const ResExtUserDataList* pExtUserDataList;
    bool isRootPaneParts;
    bool isUtf8;
    // smo
    // uint32_t resourceVersion;
    // newer
    // bool isViewerInvisibleByParent;
};

struct BuildResultInformation {
    size_t requiredUi2dConstantBufferSize;
    size_t requiredFontConstantBufferSize;

    void SetDefault();
};

// should probably have this has a util function
#define FLAG(bit) 0x1 << bit

namespace detail {
enum FrameSpecFlag {
    FrameSpecFlag_VertexEffectTexcoordEnabled = FLAG(0),
    FrameSpecFlag_VertexEffectFrame = FLAG(1),
    FrameSpecFlag_VertexEffectTexcoordAlignRight = FLAG(2),
    FrameSpecFlag_VertexEffectTexcoordAlignBottom = FLAG(3),
    FrameSpecFlag_VertexEffectDotByDotU = FLAG(4),
    FrameSpecFlag_VertexEffectDotByDotV = FLAG(5),

    FrameSpecFlag_VertexEffectFrameIdShift = 8,
    FrameSpecFlag_VertexEffectFrameIdMask = 0xF << FrameSpecFlag_VertexEffectFrameIdShift,

    FrameSpecFlag_VertexEffectTexCoordSwap = FLAG(16),
    FrameSpecFlag_VertexEffectTexCoordHFlip = FLAG(17),
    FrameSpecFlag_VertexEffectTexCoordVFlip = FLAG(18),
    FrameSpecFlag_VertexEffectConstColor = FLAG(19),

    FrameSpecFlag_FrameIdLt = 0 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdRt = 1 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdLb = 2 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdRb = 3 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdL = 4 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdR = 5 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdT = 6 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdB = 7 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdLt = 8 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdRt = 9 << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdLb = 0xA << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdRb = 0xB << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame2IdL = 0xC << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame2IdC = 0xD << FrameSpecFlag_VertexEffectFrameIdShift,

    FrameSpecFlag_Content = FrameSpecFlag_Frame2IdC,
    FrameSpecFlag_FrameHorizontalL = FrameSpecFlag_FrameIdL | FrameSpecFlag_VertexEffectFrame |
                                     FrameSpecFlag_VertexEffectTexcoordEnabled,
    FrameSpecFlag_FrameHorizontalR = FrameSpecFlag_FrameIdR | FrameSpecFlag_VertexEffectFrame |
                                     FrameSpecFlag_VertexEffectTexcoordEnabled,
    FrameSpecFlag_FrameHorizontalNoContentL = 0xC13,
    FrameSpecFlag_FrameHorizontalNoContentR = 0x503,

    FrameSpecFlag_FrameLt = FrameSpecFlag_FrameIdLt | FrameSpecFlag_VertexEffectFrame |
                            FrameSpecFlag_VertexEffectTexcoordEnabled,
    FrameSpecFlag_FrameRt = FrameSpecFlag_FrameIdRt | FrameSpecFlag_VertexEffectFrame |
                            FrameSpecFlag_VertexEffectTexcoordEnabled,

    FrameSpecFlag_FrameLb = FrameSpecFlag_FrameIdLb | FrameSpecFlag_VertexEffectFrame |
                            FrameSpecFlag_VertexEffectTexcoordEnabled,
    FrameSpecFlag_FrameRb = FrameSpecFlag_FrameIdRb | FrameSpecFlag_VertexEffectFrame |
                            FrameSpecFlag_VertexEffectTexcoordEnabled,

    FrameSpecFlag_FrameL = FrameSpecFlag_FrameIdL | FrameSpecFlag_VertexEffectFrame |
                           FrameSpecFlag_VertexEffectTexcoordEnabled |
                           FrameSpecFlag_VertexEffectDotByDotV |
                           FrameSpecFlag_VertexEffectTexcoordAlignBottom,
    FrameSpecFlag_FrameR = FrameSpecFlag_FrameIdR | FrameSpecFlag_VertexEffectFrame |
                           FrameSpecFlag_VertexEffectTexcoordEnabled |
                           FrameSpecFlag_VertexEffectDotByDotV,

    FrameSpecFlag_FrameT = FrameSpecFlag_FrameIdT | FrameSpecFlag_VertexEffectFrame |
                           FrameSpecFlag_VertexEffectTexcoordEnabled |
                           FrameSpecFlag_VertexEffectDotByDotU,
    FrameSpecFlag_FrameB = FrameSpecFlag_FrameIdB | FrameSpecFlag_VertexEffectFrame |
                           FrameSpecFlag_VertexEffectTexcoordEnabled |
                           FrameSpecFlag_VertexEffectDotByDotU |
                           FrameSpecFlag_VertexEffectTexcoordAlignRight,

    FrameSpecFlag_Frame4Lt = FrameSpecFlag_Frame4IdLt | FrameSpecFlag_VertexEffectFrame |
                             FrameSpecFlag_VertexEffectTexcoordEnabled |
                             FrameSpecFlag_VertexEffectDotByDotU,
    FrameSpecFlag_Frame4Rt = FrameSpecFlag_Frame4IdRt | FrameSpecFlag_VertexEffectFrame |
                             FrameSpecFlag_VertexEffectTexcoordEnabled |
                             FrameSpecFlag_VertexEffectDotByDotV,

    FrameSpecFlag_Frame4Lb = FrameSpecFlag_Frame4IdLb | FrameSpecFlag_VertexEffectFrame |
                             FrameSpecFlag_VertexEffectTexcoordEnabled |
                             FrameSpecFlag_VertexEffectDotByDotV |
                             FrameSpecFlag_VertexEffectTexcoordAlignBottom,
    FrameSpecFlag_Frame4Rb = FrameSpecFlag_Frame4IdRb | FrameSpecFlag_VertexEffectFrame |
                             FrameSpecFlag_VertexEffectTexcoordEnabled |
                             FrameSpecFlag_VertexEffectDotByDotU |
                             FrameSpecFlag_VertexEffectTexcoordAlignRight,

    FrameSpecFlag_FlipHFlip = FrameSpecFlag_VertexEffectTexCoordHFlip,
    FrameSpecFlag_FlipVFlip = FrameSpecFlag_VertexEffectTexCoordVFlip,
    FrameSpecFlag_FlipR90 = FrameSpecFlag_FlipHFlip | FrameSpecFlag_VertexEffectTexCoordSwap,
    FrameSpecFlag_FlipR180 = FrameSpecFlag_FlipHFlip | FrameSpecFlag_FlipVFlip,
    FrameSpecFlag_FlipR270 = FrameSpecFlag_FlipVFlip | FrameSpecFlag_VertexEffectTexCoordSwap,

    FrameSpecFlag_Normal = 0x0
};

template <int StrMax>
bool EqualsName(const char* pName1, const char* pName2) {
    for (int i = 0; i < StrMax; ++i) {
        if (pName1[i] != pName2[i]) {
            return false;
        } else if (pName1[i] == '\0') {
            return true;
        }
    }
    return true;
}

inline bool EqualsResName(const char* pName1, const char* pName2) {
    return EqualsName<ResourceNameStrMax>(pName1, pName2);
}

inline bool EqualsGroupName(const char* pName1, const char* pName2) {
    return EqualsName<GroupNameStrMax>(pName1, pName2);
}

inline bool EqualsMaterialName(const char* pName1, const char* pName2) {
    return EqualsName<28>(pName1, pName2);
}

inline const char* GetStrTableStr(const void* pStrTable, int index) {
    auto pOffsets = static_cast<const uint32_t*>(pStrTable);
    auto pStringPool = static_cast<const char*>(pStrTable);
    return &pStringPool[pOffsets[index]];
}

class TexCoordArray {
public:
    void Initialize();
    bool IsEmpty() const;
    void Free();
    void Reserve(int);
    int GetSize() const;
    void SetSize(int);
    const TexCoordQuad* GetArray() const;
    void GetCoord(util::Float2*, int) const;
    void SetCoord(int, const util::Float2*);
    void Copy(const void*, int);
    bool CompareCopiedInstanceTest(const TexCoordArray&) const;

private:
    uint8_t m_Cap;
    uint8_t m_Count;

    util::Float2 (*m_pData)[4];
};

}  // namespace detail
}  // namespace ui2d
}  // namespace nn
