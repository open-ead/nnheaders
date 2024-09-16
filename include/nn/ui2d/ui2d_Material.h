/**
 * @file Material.h
 * @brief UI Material implementation.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/ui2d/ui2d_GraphicsResource.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_TexMap.h>

namespace nn {
namespace ui2d {

class AnimTransform;
struct BuildResultInformation;
struct UserShaderInformation;
class ResourceAcessor;
struct ResMaterial;
class TexMap;

struct MaterialCopyContext {
    gfx::Device* pDevice;
    const CaptureTextureCopyInfo* pCaptureTextureCopyInfo;
    ResourceAccessor* pResAccessor;
    const char* pNewRootName;
};

namespace detail {

struct MatMemCount {
    uint32_t texMap : 2;
    uint32_t texSrt : 2;
    uint32_t texCoordGen : 2;
    uint32_t tevStage : 3;
    uint32_t alpComp : 1;
    uint32_t blendMode : 2;
    uint32_t indirectParameter : 1;
    uint32_t projectionTexGen : 2;
    uint32_t fontShadowParameter : 1;
    uint32_t detailedCombinerParameter : 1;
    uint32_t combinerUserShaderParameter : 1;

    /* remove before merge, cheatsheet
    texMap = 0x3,
    texSrt = 0xC,
    texCoordGen = 0x30,
    tevStage = 0x1C0,
    alpComp = 0x200,
    blendMode = 0xC00,
    indirectParameter = 0x1000,
    projectionTexGen = 0x6000,
    fontShadowParameter = 0x8000,
    detailedCombinerParameter = 0x10000,
    combinerUserShaderParameter = 0x20000,
    */
};

// todo: confirm this math

// 16 * (v17 & 3)
inline int CalculateOffsetTexSrtArray(const MatMemCount& bitCount) {
    return bitCount.texMap * sizeof(TexMap);
}

// 20 * ((m_MemCap >> 2) & 3)
inline int CalculateOffsetTexCoordGenArray(const MatMemCount& bitCount) {
    return CalculateOffsetTexSrtArray(bitCount) + (bitCount.texSrt * sizeof(ResTexSrt));
}

// (m_MemCap & 0x30)
inline int CalculateOffsetAlphaCompare(const MatMemCount& bitCount) {
    return CalculateOffsetTexCoordGenArray(bitCount) +
           (bitCount.texCoordGen * sizeof(ResTexCoordGen));
}

// (m_MemCap >> 6) & 8
inline int CalculateOffsetBlendMode(const MatMemCount& bitCount) {
    return CalculateOffsetAlphaCompare(bitCount) + (bitCount.alpComp * sizeof(ResAlphaCompare));
}

// (m_MemCap >> 8) & 0xC
inline int CalculateOffsetIndirectParameter(const MatMemCount& bitCount) {
    return CalculateOffsetBlendMode(bitCount) + (bitCount.blendMode * sizeof(ResBlendMode));
}

// -((m_MemCap >> 12) & 1) & 0xC
inline int CalculateOffsetTevStageArray(const MatMemCount& bitCount) {
    return CalculateOffsetIndirectParameter(bitCount) +
           (bitCount.indirectParameter * sizeof(ResIndirectParameter));
}

// (m_MemCap >> 4) & 0x1C
inline int CalculateOffsetProjectionTexGen(const MatMemCount& bitCount) {
    return CalculateOffsetTevStageArray(bitCount) + (bitCount.tevStage * sizeof(ResTevStage));
}

// 20 * ((m_MemCap >> 2) & 3)
inline int CalculateOffsetFontShadowParameter(const MatMemCount& bitCount) {
    return CalculateOffsetProjectionTexGen(bitCount) +
           (bitCount.projectionTexGen * sizeof(ResProjectionTexGenParameters));
}

inline int CalculateOffsetDetailedCombinerStageInfo(const MatMemCount& bitCount) {
    return CalculateOffsetFontShadowParameter(bitCount) +
           (bitCount.fontShadowParameter * sizeof(ResFontShadowParameter));
}

inline int CalculateOffsetDetailedCombinerStage(const MatMemCount& bitCount) {
    return CalculateOffsetDetailedCombinerStageInfo(bitCount);
}

inline int CaluculateOffsetCombinerUserShader(const MatMemCount& bitCount) {
    return CalculateOffsetDetailedCombinerStage(bitCount);
}

}  // namespace detail

class Material {
    enum Flags {
        Flags_UserAllocated,
        Flags_TextureOnly,
        Flags_ThresholdingAlphaInterpolation,
        Flags_BlackColorFloat,
        Flags_WhiteColorFloat,
        Flags_DynamicAllocatedColorData,
        Flags_Use2ForShaderVariationCountPerBlend,
        Flags_UseDetailedCombinerWithVariationVariable,
        Flags_MaxFlags
    };

public:
    Material();
    Material(BuildResultInformation*, gfx::Device*, const ResMaterial*, const ResMaterial*,
             const BuildArgSet&);
    Material(BuildResultInformation*, gfx::Device*, const ResMaterial*, const ResMaterial*,
             const BuildArgSet&, CaptureTextureCopyInfo*);
    Material(const Material&, gfx::Device*);
    Material(const Material&, MaterialCopyContext&);

    void CopyMaterialImpl(const Material&, MaterialCopyContext&);
    virtual ~Material();
    void Finalize(gfx::Device*);

    bool ReserveMem(int, int, int, int, bool, int, bool, int, bool, bool, bool);
    bool ReserveMem(int, int, int, int, bool, int, bool, int, bool, bool);
    bool ReserveMem(int, int, int, int, bool, int, bool, int, bool);
    bool ReserveMem(int, int, int, int, bool, int, bool, int);
    bool ReserveMem(int, int, int, int, bool, int, bool);
    bool ReserveMem(int, int, int, int, bool, int);
    bool ReserveMem(int, int, int, int, bool);
    bool ReserveMem(int, int, int, int);
    bool ReserveMem(int, int, int);
    uint8_t GetTexMapCap() const { return m_MemCap.texMap; }
    uint8_t GetTexSrtCap() const { return m_MemCap.texSrt; }
    uint8_t GetTexCoordGenCap() const;
    uint8_t GetTevStageCap() const;
    bool IsAlphaCompareCap() const { return m_MemCap.alpComp; }
    bool IsBlendModeCap() const;
    bool IsBlendModeAlphaCap() const;
    bool IsIndirectParameterCap() const { return m_MemCap.indirectParameter; }
    bool IsFontShadowParameterCap() const { return m_MemCap.fontShadowParameter; }
    bool UseDetailedCombinerCap() const { return m_MemCap.detailedCombinerParameter; }

    bool UseCombinerUserShaderCap() const;
    bool IsWhiteColorFloat() const;
    bool IsInterpolateColorFloat(InterpolateColor type) const;

    const char* GetName() const { return m_pName; }

    void SetName(const char*);
    bool GetTextureOnly() const;
    void SetTextureOnly(bool);
    bool GetThresholdingAlphaInterpolation() const;
    void SetThresholdingAlphaInterpolation(bool);
    ShaderId GetShaderId() const;
    void SetShaderId(ShaderId);
    bool IsUserShaderAssigned() const;

    uint8_t GetTexMapCount() const { return m_MemCount.texMap; }

    void SetTexMapCount(uint8_t);
    uint8_t GetTexCoordGenCount() const;
    void SetTexCoordGenCount(uint8_t);
    uint8_t GetTexSrtCount() const;
    void SetTexSrtCount(uint8_t);
    static void CalculateTextureMtx(float (*)[3], const ResTexSrt&, const TexMap&);
    static void CalculateIndirectMtx(float (*)[3], const float, const ResVec2&);

    const TexMap& GetTexMap(int) const;
    void AppendTexMap(const TextureInfo&);

    void SetTexMap(int idx, const TextureInfo* pTexInfo) {
        GetTexMapArray()[idx].SetTextureInfo(pTexInfo);
    }

    void SetTexMapWrapMode(int, TexWrap, TexWrap);
    void SetTexMapFilter(int, TexFilter, TexFilter);

    const ResTexSrt& GetTexSrt(int) const;
    void SetTexSrt(int, const ResTexSrt&);
    const ResTexCoordGen& GetTexCoordGen(int) const;
    void SetTexCoordGen(int, const ResTexCoordGen&);
    uint8_t GetProjectionTexGenCount() const;
    void SetProjectionTexGenCount(uint8_t);
    float GetTexSrtElement(int, int) const;

    void SetTexSrtElement(int idx, int elementIdx, float value) {
        auto const srtAny = util::BytePtr(GetTexSrtArray(), idx * sizeof(ResTexSrt)).Get<float>();
        srtAny[elementIdx] = value;
    }

    uint8_t GetTevStageCount() const;
    void SetTevStageCount(uint8_t);
    const util::Unorm8x4& GetColor(int) const;
    const util::Float4& GetColorAsFloat(int) const;
    void GetColorWithFloatConversion(util::Float4&, int) const;
    void SetColor(int, const util::Unorm8x4&);
    void SetColor(int, const util::Float4&);
    int GetColorElement(int) const;
    float GetColorElementFloat(int) const;

    void SetColorElement(int colorType, int value) {
        if (colorType >= AnimTargetMatColor_MaxAnimTargetMatColor) {
            return;
        }

        uint32_t idx = colorType / 4;
        if (idx < 2) {
            switch (colorType % 4) {
            case 0:
                m_Colors.byteColor[idx].v[0] = value;
                break;
            case 1:
                m_Colors.byteColor[idx].v[1] = value;
                break;
            case 2:
                m_Colors.byteColor[idx].v[2] = value;
                break;
            case 3:
                m_Colors.byteColor[idx].v[3] = value;
                break;
            }
        } else if (idx < 7) {
            if (UseDetailedCombinerCap()) {
                ResDetailedCombinerStageInfo* pInfo = GetDetailedCombinerStageInfoPtr();

                idx -= 2;
                ResColor color = pInfo->GetConstantColor(idx);

                switch (colorType % 4) {
                case 0:
                    color.r = value;
                    break;
                case 1:
                    color.g = value;
                    break;
                case 2:
                    color.b = value;
                    break;
                case 3:
                    color.a = value;
                    break;
                }

                pInfo->SetConstantColor(idx, color);
            }
        }
    }

    void SetColorElementFloat(int colorType, float value) {
        colorType -= AnimTargetMatColor_MaxAnimTargetMatColor;

        uint32_t idx = colorType / 4;
        if (idx < 2) {
            switch (colorType % 4) {
            case 0:
                m_Colors.pFloatColor[idx].v[0] = value;
                break;
            case 1:
                m_Colors.pFloatColor[idx].v[1] = value;
                break;
            case 2:
                m_Colors.pFloatColor[idx].v[2] = value;
                break;
            case 3:
                m_Colors.pFloatColor[idx].v[3] = value;
                break;
            default:
                break;
            }
        }
    }

    void SetIndirectSrtElement(int eleIdx, float value) {
        auto const srtAny = util::BytePtr(GetIndirectParameterPtr()).Get<float>();
        srtAny[eleIdx] = value;
    }

    const ResTevStage& GetTevStage(int) const;
    void SetTevStage(int, const ResTevStage&);
    const ResDetailedCombinerStage& GetDetailedCombinerStage(int) const;
    void SetDetailedCombinerStage(int, const ResDetailedCombinerStage&);
    const ResColor GetConstantColor(int) const;
    void SetConstantColor(int, util::Unorm8x4);
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectRgb() const;
    void SetBufferColorSelectRgb(ResDetailedCombinerStage::ConstColorSelect);
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelAlpha() const;
    void SetBufferColorSelAlpha(ResDetailedCombinerStage::ConstColorSelect);

    const ResAlphaCompare& GetAlphaCompare() const { return *GetAlphaComparePtr(); }
    void SetAlphaCompare(const ResAlphaCompare& value) { *GetAlphaComparePtr() = value; }

    const ResBlendMode& GetBlendMode() const;
    void SetBlendMode(ResBlendMode);
    const ResBlendMode& GetBlendModeAlpha() const;
    void SetBlendModeAlpha(ResBlendMode);
    void InitializeBlendInformation(gfx::Device*);
    void FinalizeBlendInformation(gfx::Device*);
    const ResIndirectParameter& GetIndirectParameter() const;
    void SetIndirectParameter(const ResIndirectParameter&);
    void SetProjectionTexGenParameters(int, const ResProjectionTexGenParameters&);
    void SetProjectionTexGenParamaters(int, const ResProjectionTexGenParameters&);
    const ResProjectionTexGenParameters& GetProjectionTexGenParameters(int) const;
    const ResProjectionTexGenParameters& GetProjectionTexGenParamaters(int) const;
    void SetFontShadowParameterCount(uint8_t);
    const ResFontShadowParameter& GetFontShadowParameter() const;
    void SetFontShadowParameter(const ResFontShadowParameter&);

    void SetFontShadowParameterElement(int eleIdx, uint8_t value) {
        auto const elements = util::BytePtr(GetFontShadowParameterPtr()).Get<uint8_t>();
        elements[eleIdx] = value;
    }

    virtual void BindAnimation(AnimTransform*);
    virtual void UnbindAnimation(AnimTransform*);
    bool IsUserAllocated() const;
    void SetUserAllocated();
    virtual void SetupGraphics(DrawInfo&, uint8_t, ShaderVariation, bool,
                               const util::MatrixT4x3fType&, const Size*, const ResExtUserData*,
                               uint16_t);
    void SetupSubmaterialOf_TextureMatrix(DrawInfo&, const util::MatrixT4x3fType&, const Size*);
    void SetupSubmaterialOf_TextureMatrix(DrawInfo&, const util::MatrixT4x3fType&);
    void SetupSubmaterialOf_TextureProjectionMatrix(DrawInfo&, const util::MatrixT4x3fType&,
                                                    const Size*,
                                                    const ResProjectionTexGenParameters&,
                                                    int) const;
    void SetRcpTexSize(const DrawInfo&) const;
    void SetupSubmaterialOf_Texture(DrawInfo&, gfx::CommandBuffer&) const;
    void SetupSubmaterialOf_Tev(DrawInfo&, const ResExtUserData*, uint16_t, const Size*) const;
    void SetupSubmaterialOf_DetailedCombiner(DrawInfo&) const;
    void SetupSubmaterialOf_CombinerUserShader(DrawInfo&, const ResExtUserData*, uint16_t,
                                               const Size*) const;
    void SetAllowableTextureQuantity(const DrawInfo&);

    struct ConstantBufferForVertexShader {
        float projection[4][4];
        float modelView[3][4];
        float texMtx0XZ[4];
        float texMtx0YW[4];
        float texMtx1XZ[4];
        float texMtx1YW[4];
        float texMtx2XZ[4];
        float texMtx2YW[4];
        float color[4];
        float vertexTexCoord0[4][4];
        float vertexTexCoord1[4][4];
        float vertexTexCoord2[4][4];
        float frameSize[4];
        float paneSize[2];
        float padding0[2];
        float rcpTexSize0[4];
        float vertexColor[4][4];
        float transform[4];
        int generatingTexCoord[3];
        int frameSpec;
    };

    struct ConstantBufferForPixelShader {
        float interpolateWidth[4];
        float interpolateOffset[4];
        float indirectMtx0[4];
        float indirectMtx1[4];
        float vertexColor[4][4];
        int textureMode;
        int colorEffectMode;
        float padding0[2];
    };

    struct ConstantBufferForDetailedCombinerPixelShader {
        int stageCountMax;
        float padding1[3];
        float constantColor[7][4];
        int stage_bit[6][4];
    };

    struct ConstantBufferForCombinerUserShaderPixelShader {
        float fixedZero;
        float paneWidth;
        float paneHeight;
        int padding[1];
        float constantColor[7][4];
        float modelViewMatrix[3][4];
        float modelMatrix[3][4];
        float viewMatrix[3][4];
        float cameraPosition[3];
        float padding2[1];
        int userData_int[4];
        int userData_ivec2[4][4];
        int userData_rgba[4][4];
        float userData_float[4];
        float userData_vec2[4][4];
        float userData_vec3[4][4];
    };

    void AllocateConstantBuffer(DrawInfo&);
    void SetupBlendState(const DrawInfo*);
    void* GetConstantBufferForUserVertexShader(const DrawInfo&) const;
    void* GetConstantBufferForUserPixelShader(const DrawInfo&) const;
    void* GetConstantBufferForUserGeometryShader(const DrawInfo&) const;
    ConstantBufferForDetailedCombinerPixelShader*
    GetConstantBufferForDetailedCombinerPixelShader(const DrawInfo&) const;
    ConstantBufferForCombinerUserShaderPixelShader*
    GetConstantBufferForCombinerUserShaderPixelShader(const DrawInfo&) const;
    ConstantBufferForVertexShader* GetConstantBufferForVertexShader(const DrawInfo&) const;
    ConstantBufferForPixelShader* GetConstantBufferForPixelShader(const DrawInfo&) const;
    void SetCommandBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyVertexShaderConstantBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyGeometryShaderConstantBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyPixelShaderConstantBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyPixelShaderConstantBufferDefault(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyPixelShaderDetailedCombinerConstantBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void ApplyPixelShaderCombinerUserShaderConstantBuffer(gfx::CommandBuffer&, DrawInfo&) const;
    void SetCommandBufferOnlyBlend(gfx::CommandBuffer&) const;
    void SetShader(gfx::CommandBuffer&) const;
    void EnableAlphaTest(gfx::CommandBuffer&) const;
    void DisableAlphaTest() const;
    int GetConstantBufferSlotForVertexShader() const;
    bool CompareCopiedInstanceTest(const Material&) const;
    const ShaderInfo* GetShaderInfo() const;
    uint8_t GetShaderVariation() const;

protected:
    const TexMap* GetTexMapArray() const;

    TexMap* GetTexMapArray() { return util::BytePtr(m_pMem).Get<TexMap>(); }

    const ResTexSrt* GetTexSrtArray() const;

    ResTexSrt* GetTexSrtArray() {
        return util::BytePtr(m_pMem, detail::CalculateOffsetTexSrtArray(m_MemCap)).Get<ResTexSrt>();
    }

    const ResTexCoordGen* GetTexCoordGenArray() const;
    ResTexCoordGen* GetTexCoordGenArray();

    const ResAlphaCompare* GetAlphaComparePtr() const {
        return util::ConstBytePtr(m_pMem, detail::CalculateOffsetAlphaCompare(m_MemCap))
            .Get<ResAlphaCompare>();
    }

    ResAlphaCompare* GetAlphaComparePtr() {
        return util::BytePtr(m_pMem, detail::CalculateOffsetAlphaCompare(m_MemCap))
            .Get<ResAlphaCompare>();
    }

    const ResBlendMode* GetBlendModePtr() const;
    ResBlendMode* GetBlendModePtr();
    const ResBlendMode* GetBlendModeAlphaPtr() const;
    ResBlendMode* GetBlendModeAlphaPtr();
    const ResIndirectParameter* GetIndirectParameterPtr() const;

    ResIndirectParameter* GetIndirectParameterPtr() {
        return util::BytePtr(m_pMem, detail::CalculateOffsetIndirectParameter(m_MemCap))
            .Get<ResIndirectParameter>();
    }

    ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoParameterPtr();
    ResDetailedCombinerStage* GetDetailedCombinerStageParameterPtr();
    const ResProjectionTexGenParameters* GetProjectionTexGenArray() const;
    ResProjectionTexGenParameters* GetProjectionTexGenArray();
    const ResTevStage* GetTevStageArray() const;
    ResTevStage* GetTevStageArray();
    const ResFontShadowParameter* GetFontShadowParameterPtr() const;

    ResFontShadowParameter* GetFontShadowParameterPtr() {
        return util::BytePtr(m_pMem, detail::CalculateOffsetFontShadowParameter(m_MemCap))
            .Get<ResFontShadowParameter>();
    }

    int GetProjectionTexGenParametersIdxFromTexCoordGenIdx(int) const;
    int GetProjectionTexGenParamatersIdxFromTexCoordGenIdx(int) const;

    const ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoPtr() const;

    ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoPtr() {
        return util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStageInfo(m_MemCap))
            .Get<ResDetailedCombinerStageInfo>();
    }

    const ResDetailedCombinerStage* GetDetailedCombinerStageAry() const;
    ResDetailedCombinerStage* GetDetailedCombinerStageAry();
    ResCombinerUserShader* GetCombinerUserShaderPtr();
    const ResCombinerUserShader* GetCombinerUserShaderPtr() const;

private:
    struct UserShaderConstantBufferInformation {
        uint32_t m_ConstantBufferExtendAreaSizeForVertexShader;
        uint32_t m_ConstantBufferExtendAreaSizeForPixelShader;
        uint32_t m_ConstantBufferExtendAreaSizeForGeometryShader;
        uint32_t m_GeometryShaderConstantBufferOffset;

        void SetDefault();
    };

    void Initialize();
    void InitializeMaterialImpl(BuildResultInformation*, gfx::Device*, const ResMaterial*,
                                const ResMaterial*, const BuildArgSet&, CaptureTextureCopyInfo*);
    void InitializeMatMemCount(detail::MatMemCount*) const;
    void GetShaderBlend(int*, int*) const;
    void GetShaderForDetailedCombiner(int*, int*) const;
    void SetupShader(gfx::Device*, const BuildArgSet&, const BuildResSet*);
    bool IsIndirectBlendUsed() const;
    bool IsPerspectiveTextureProjectionUsed() const;
    void SetupUserShaderConstantBufferInformation(const UserShaderInformation&);
    void CopyUserShaderConstantBufferInformation(const Material&);
    void CollectConstantBufferSize(BuildResultInformation*, gfx::Device*) const;
    void InitializeBlendInformationImpl(gfx::Device*);
    void FinalizeBlendInformationImpl(gfx::Device*);
    PresetBlendStateId GetBlendStateId() const;
    size_t GetVertexShaderConstantBufferSize() const;
    size_t GetGeometryShaderConstantBufferSize() const;
    size_t GetPixelShaderConstantBufferSize() const;
    size_t GetPixelShaderDetailedCombinerConstantBufferSize() const;
    size_t GetPixelShaderCombinerUserShaderConstantBufferSize() const;
    size_t CalculateReserveMemSize(int, int, int, int, int, int, int, int, int, int, int);
    size_t CalculateReserveMemSize(detail::MatMemCount&);
    void ResParameterToMemory(int, int, int, int, int, int, int, int, int, int, int);
    const Material& operator=(const Material&);

    union {
        util::Unorm8x4 byteColor[2];
        util::Float4* pFloatColor;
    } m_Colors;
    detail::MatMemCount m_MemCap;
    detail::MatMemCount m_MemCount;
    void* m_pMem;
    const ShaderInfo* m_pShaderInfo;
    const char* m_pName;
    uint32_t m_VertexShaderConstantBufferOffset;
    uint32_t m_PixelShaderConstantBufferOffset;
    UserShaderConstantBufferInformation* m_pUserShaderConstantBufferInformation;
    gfx::BlendState* m_pBlendState;
    int8_t m_AllowableTextureQuantity;
    uint8_t m_ShaderId;
    uint8_t m_Flag;
    uint8_t m_ShaderVariation;
};

}  // namespace ui2d
}  // namespace nn
