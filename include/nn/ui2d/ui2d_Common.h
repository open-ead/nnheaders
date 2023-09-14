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

namespace detail {
enum FrameSpecFlag {
    FrameSpecFlag_VertexEffectTexcoordEnabled = 0x1,
    FrameSpecFlag_VertexEffectFrame = 0x2,
    FrameSpecFlag_VertexEffectTexcoordAlignRight = 0x4,
    FrameSpecFlag_VertexEffectTexcoordAlignBottom = 0x8,
    FrameSpecFlag_VertexEffectDotByDotU = 0x10,
    FrameSpecFlag_VertexEffectDotByDotV = 0x20,
    FrameSpecFlag_VertexEffectFrameIdShift = 8,
    FrameSpecFlag_VertexEffectFrameIdMask = 0xf << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_VertexEffectTexCoordSwap = 0x10000,
    FrameSpecFlag_VertexEffectTexCoordHFlip = 0x20000,
    FrameSpecFlag_VertexEffectTexCoordVFlip = 0x40000,
    FrameSpecFlag_VertexEffectConstColor = 0x40000000,

    FrameSpecFlag_FrameIdLt = 0,
    FrameSpecFlag_FrameIdRt = 0x100,
    FrameSpecFlag_FrameIdLb = 0x200,
    FrameSpecFlag_FrameIdRb = 0x300,
    FrameSpecFlag_FrameIdL = 1024,
    FrameSpecFlag_FrameIdR = 1280,
    FrameSpecFlag_FrameIdT = 1536,
    FrameSpecFlag_FrameIdB = 1792,
    FrameSpecFlag_Frame4IdLt = 2048,
    FrameSpecFlag_Frame4IdRt = 2304,
    FrameSpecFlag_Frame4IdLb = 2560,
    FrameSpecFlag_Frame4IdRb = 2816,
    FrameSpecFlag_Frame2IdL = 3072,
    FrameSpecFlag_Frame2IdC = 3328,
    FrameSpecFlag_Content = 3328,
    FrameSpecFlag_FrameHorizontalL = 1027,
    FrameSpecFlag_FrameHorizontalR = 1283,
    FrameSpecFlag_FrameHorizontalNoContentL = 3091,
    FrameSpecFlag_FrameHorizontalNoContentR = 1283,
    FrameSpecFlag_FrameLt = 3,
    FrameSpecFlag_FrameRt = 259,
    FrameSpecFlag_FrameLb = 515,
    FrameSpecFlag_FrameRb = 771,
    FrameSpecFlag_FrameL = 1067,
    FrameSpecFlag_FrameR = 1315,
    FrameSpecFlag_FrameT = 1555,
    FrameSpecFlag_FrameB = 1815,
    FrameSpecFlag_Frame4Lt = 2067,
    FrameSpecFlag_Frame4Rt = 2339,
    FrameSpecFlag_Frame4Lb = 2603,
    FrameSpecFlag_Frame4Rb = 2839,
    FrameSpecFlag_FlipHFlip = 131072,
    FrameSpecFlag_FlipVFlip = 262144,
    FrameSpecFlag_FlipR90 = 196608,
    FrameSpecFlag_FlipR180 = 393216,
    FrameSpecFlag_FlipR270 = 327680,
    FrameSpecFlag_Normal = 0
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
