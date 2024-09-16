#pragma once

#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_BytePtr.h>
#include <nn/util/util_MathTypes.h>

namespace nn {
namespace font {

class GpuBuffer;
class TextureObject;

namespace detail {

static const int UseTextureCountMax = 8;
static const int VertexCountByLetter = 4;

enum BitFlags { BitFlags_ColorBlackWhiteInterpolationEnabled = 1, BitFlags_BorderEffectEnabled };

struct TextureUseInfo {
    const TextureObject* pTexObj;
    uint32_t useCount;
    uint32_t bitFlags;
};

struct Vertex {
    util::Float3 xyz;
    util::Unorm8x4 color;
    util::Float3 uvw;

    void Set(float, float, float, const util::Unorm8x4&, float, float, float);
};

struct VertexShaderCharAttribute {
    util::Float4 posAndSize;
    util::Float4 texCoord;
    util::Unorm8x4 color[2];
    int32_t sheetIndex;
    float italicOffset;
    util::Float4 translate;

    void Set(const float, const float, const float, const float, const util::Float4&,
             const util::Unorm8x4&, const util::Unorm8x4&, const int32_t, const float, const float);
};

struct VertexShaderCharAttributeWithTransform {
    util::Float4 posAndSize;
    util::Float4 texCoord;
    util::Unorm8x4 color[2];
    int32_t sheetIndex;
    float italicOffset;
    util::Float4 translate;
    util::Float4 rotateMatrixAndCenterX;
    util::Float4 rotateMatrixAndCenterY;

    void Set(const float, const float, const float, const float, const util::Float4&,
             const util::Unorm8x4&, const util::Unorm8x4&, const int32_t, const float, const float,
             const util::Float4&, const util::Float4&, const util::Float4&);
};
}  // namespace detail

class CharAttribute {
public:
    enum Flags { Flags_BorderEffect = 1 };

    util::Float4 pos;
    util::Unorm8x4 color[2];
    util::Float4 tex;

private:
    uintptr_t m_pTexObjAndFlags;

public:
    int16_t italicOffset;
    uint8_t sheetIndex;
    uint8_t shadowAlpha;

    void SetTexObjAndFlags(const TextureObject*, uint8_t);
    const TextureObject* GetTexObj() const;
    uint8_t GetFlags() const;
};

struct ShadowParameter {
    util::Unorm8x4 shadowUpperColor;
    util::Unorm8x4 shadowLowerColor;
    util::Float2 shadowOffset;
    util::Float2 shadowScale;
    float shadowItalicOffset;
};

class PerCharacterTransformInfo {
public:
    float Scale[2];
    float RotationCos[3];
    float RotationSin[3];
    float Translation[3];
    uint8_t LT[4];
    uint8_t LB[4];

    PerCharacterTransformInfo();
};

class ConstantBufferAdditionalContent {
public:
    enum Flags {
        Flags_InvisibleBorder = 1,
        Flags_ShadowEnabled,
        Flags_PerCharacterTransformEnabled = 4
    };

    enum PerCharacterTransformCenter {
        PerCharacterTransformCenter_Center,
        PerCharacterTransformCenter_Bottom,
        PerCharacterTransformCenter_Count
    };

public:
    ConstantBufferAdditionalContent();
    ConstantBufferAdditionalContent& SetViewMatrix(const util::MatrixT4x3fType*);
    ConstantBufferAdditionalContent& SetLocalMatrix(const util::MatrixT4x3fType*);
    ConstantBufferAdditionalContent& SetInterpolateBlack(const util::Unorm8x4&);
    ConstantBufferAdditionalContent& SetInterpolateBlack(const util::Float4&);
    ConstantBufferAdditionalContent& SetInterpolateWhite(const util::Unorm8x4&);
    ConstantBufferAdditionalContent& SetInterpolateWhite(const util::Float4&);
    ConstantBufferAdditionalContent& SetInterpolateAlpha(uint8_t);
    ConstantBufferAdditionalContent& SetShadowInterpolateBlack(const util::Unorm8x4&);
    ConstantBufferAdditionalContent& SetShadowInterpolateBlack(const util::Float4&);
    ConstantBufferAdditionalContent& SetShadowInterpolateWhite(const util::Unorm8x4&);
    ConstantBufferAdditionalContent& SetShadowInterpolateWhite(const util::Float4&);
    ConstantBufferAdditionalContent& SetShadowInterpolateAlpha(uint8_t);
    const util::Unorm8x4 GetInterpolateBlack() const;
    const util::Unorm8x4 GetInterpolateWhite() const;
    uint8_t GetInterpolateAlpha() const;
    const util::Float4& GetInterpolateBlackFloat() const;
    const util::Float4& GetInterpolateWhiteFloat() const;
    const util::Unorm8x4 GetShadowInterpolateBlack() const;
    const util::Unorm8x4 GetShadowInterpolateWhite() const;
    uint8_t GetShadowInterpolateAlpha() const;
    const util::Float4& GetShadowInterpolateBlackFloat() const;
    const util::Float4& GetShadowInterpolateWhiteFloat() const;
    ConstantBufferAdditionalContent& SetBorderVisibility(bool);
    ConstantBufferAdditionalContent& SetShadowParam(const ShadowParameter*);
    ConstantBufferAdditionalContent&
    SetPerCharacterTransformInfos(const PerCharacterTransformInfo*);
    ConstantBufferAdditionalContent& SetPerCharacterTransformCenter(PerCharacterTransformCenter);
    ConstantBufferAdditionalContent& SetPerCharacterTransformCenterOffset(float);

private:
    const ShadowParameter* m_pShadowParam;
    const PerCharacterTransformInfo* m_pPerCharacterTransformInfos;
    PerCharacterTransformCenter m_PerCharacterTransformCenter;
    float m_PerCharacterTransformCenterOffset;
    const util::MatrixT4x3fType* m_pViewMatrix;
    const util::MatrixT4x3fType* m_pLocalMatrix;
    util::Float4 m_InterpolateBlackColor;
    util::Float4 m_InterpolateWhiteColor;
    uint8_t m_InterpolateWhiteAlpha;
    util::Float4 m_ShadowInterpolateBlackColor;
    util::Float4 m_ShadowInterpolateWhiteColor;
    uint8_t m_ShadowInterpolateWhiteAlpha;
    uint32_t m_Flags;
};

class DispStringBuffer {
public:
    struct ShaderParam {
        float m_Mtx[4][4];
        float m_InterpolateWidth[4];
        float m_InterpolateOffset[4];
        float m_ShadowInterpolateWidth[4];
        float m_ShadowInterpolateOffset[4];
    };

    class InitializeArg {
    public:
        InitializeArg();
        InitializeArg& SetCharCountMax(int);
        InitializeArg& SetShadowEnabled(bool);
        InitializeArg& SetPerCharacterTransformEnabled(bool);
        InitializeArg& SetPerCharacterTransformAutoShadowAlpha(bool);
        InitializeArg& SetDoubleDrawnBorder(bool);
        InitializeArg& SetDrawBuffer(void*);
        InitializeArg& SetConstantBuffer(GpuBuffer*);

    private:
        int GetCharCountMax() const;
        void* GetDrawBuffer() const;

        void* m_pDrawBuffer;
        GpuBuffer* m_pConstantBuffer;
        int m_CharCountMax;
        bool m_ShadowEnabled;
        bool m_DoubleDrawnBorder;
        bool m_PerCharacterTransformEnabled;
        bool m_PerCharacterTransformAutoShadowAlpha;
    };

    DispStringBuffer();
    ~DispStringBuffer();
    static size_t GetRequiredDrawBufferSize(const InitializeArg&);
    static size_t GetRequiredConstantBufferSize(gfx::Device*, const InitializeArg&);
    bool Initialize(gfx::Device*, const InitializeArg&);
    bool IsInitialized() const;
    void Finalize(gfx::Device*);
    int GetCharCountMax() const;
    int GetCharCount() const;
    void SetCharCount(int);
    CharAttribute* GetCharAttributes() const;
    void SetCharAttributes(CharAttribute*);
    void* GetDrawBuffer() const;
    void SetConstantBuffer(GpuBuffer*);
    void GetConstantBufferGpuAddress(gfx::GpuAddress*) const;
    void BuildConstantBuffer(const util::MatrixT4x4fType&, const ConstantBufferAdditionalContent*,
                             bool, bool);
    void BuildConstantBuffer(const util::MatrixT4x4fType&, const ConstantBufferAdditionalContent*,
                             bool);
    void BuildConstantBuffer(const util::MatrixT4x4fType&, const ConstantBufferAdditionalContent*);
    void BuildConstantBuffer(const util::MatrixT4x4fType&);
    void SetFontHeight(float);
    bool CompareCopiedInstanceTest(const DispStringBuffer&) const;
    void SetDoubleDrawnBorderEnabled(bool);
    bool IsDoubleDrawnBorderEnabled() const;

protected:
    void BuildTextureUseInfos(bool);

private:
    void BuildCommonConstantBufferData(ShaderParam&, const util::MatrixT4x4fType&,
                                       const ConstantBufferAdditionalContent&) const;
    void BuildPerCharacterAttributeConstantBuffer(const ConstantBufferAdditionalContent&, bool,
                                                  bool);
    void BuildPerCharacterParams(ptrdiff_t, const PerCharacterTransformInfo*,
                                 const ConstantBufferAdditionalContent::PerCharacterTransformCenter,
                                 const float, const ShadowParameter*, bool, bool);
    void BuildShadowBufferPerCharacterParams(
        ptrdiff_t, const PerCharacterTransformInfo*,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter, const float,
        const ShadowParameter*, const uint32_t*, util::BytePtr, uint32_t, uint32_t*, int, bool);
    void BuildCharacterBufferPerCharacterParams(
        ptrdiff_t, const PerCharacterTransformInfo*,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter, const float,
        const uint32_t*, util::BytePtr, uint32_t, uint32_t*, int, bool);
    void CalculatePerCharacterTransform(
        util::Float4&, util::Float4&, util::Float4&, const float, const float, const float,
        const float, const float, const float, const PerCharacterTransformInfo&,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter, const float) const;

    friend class RectDrawer;

    class VertexBufferData {
    public:
        detail::TextureUseInfo textureUseInfos[8];
        int textureUseInfoPos;
    };

    uint32_t m_ConstantBufferOffset;
    uint32_t m_PerCharacterParamOffset;
    uint32_t m_DrawContentFlags;
    VertexBufferData m_VertexBufferData;
    int32_t m_CharCountMax;
    int32_t m_CharCount;
    CharAttribute* m_pCharAttrs;
    uint8_t* m_pCharUseTextureIndices;
    GpuBuffer* m_pConstantBuffer;
    bool m_ShadowEnabled;
    bool m_DoubleDrawnBorder;
    bool m_PerCharacterTransformEnabled;
    bool m_PerCharacterTransformAutoShadowAlpha;
    float m_FontHeight;
};

}  // namespace font
}  // namespace nn
