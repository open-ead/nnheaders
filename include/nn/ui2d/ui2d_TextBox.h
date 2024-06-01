#pragma once

#include <nn/font/font_TagProcessorBase.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_TextSearcher.h>

namespace nn {

namespace font {
class DispStringBuffer;
class Font;
class TextWriter;
class WideTextWriter;
class PerCharacterTransformInfo;
class ConstantBufferAdditionalContent;
}  // namespace font

namespace ui2d {

class Material;
struct ResAnimationTarget;
struct ResTextBox;
struct ResAnimationInfo;

class TextBox : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane);

    typedef font::TagProcessorBase<uint16_t> TagProcessor;
    typedef font::TagProcessorBase<char> TagProcessorUtf8;

    TextBox();
    TextBox(bool);

    struct InitializeStringParam {
        int textBoxFlag;
        Layout* pResourceLayout;
        const uint16_t* pBlockText;
        size_t allocStrBufLen;
        int resStrLen;
    };

    TextBox(BuildResultInformation*, gfx::Device*, InitializeStringParam*, const ResTextBox*,
            const ResTextBox*, const BuildArgSet&);
    TextBox(const TextBox&, gfx::Device*);
    TextBox(const TextBox&, gfx::Device*, ResourceAccessor*, const char*);
    TextBox(const TextBox&, gfx::Device*, uint16_t);
    TextBox(const TextBox&, gfx::Device*, uint16_t, ResourceAccessor*, const char*);
    virtual void InitializeString(BuildResultInformation*, gfx::Device*, const BuildArgSet&,
                                  const InitializeStringParam&);
    virtual ~TextBox();
    virtual void Finalize(gfx::Device*);
    const uint16_t* GetString() const;
    const char* GetStringUtf8() const;
    const uint16_t* GetStringBuffer() const;
    const char* GetStringBufferUtf8() const;
    uint16_t GetStringLength() const;
    uint16_t GetStringBufferLength() const;
    uint16_t GetDrawStringBufferLength() const;
    virtual void AllocateStringBuffer(gfx::Device*, uint16_t);
    virtual void AllocateStringBuffer(gfx::Device*, uint16_t, uint16_t);
    virtual void FreeStringBuffer(gfx::Device*);
    virtual uint16_t SetString(const uint16_t*, uint16_t);
    virtual uint16_t SetString(const uint16_t*, uint16_t, uint16_t);
    virtual uint16_t SetStringUtf8(const char*, uint16_t);
    virtual uint16_t SetStringUtf8(const char*, uint16_t, uint16_t);
    const util::Unorm8x4& GetTextColor(int) const;
    void SetTextColor(int, const util::Unorm8x4&);
    void SetTextColor(const util::Unorm8x4&, const util::Unorm8x4&);
    const font::Font* GetFont() const;
    void SetFont(const font::Font*);
    const Size& GetFontSize() const;
    void SetFontSize(const Size&);
    float GetLineSpace() const;
    void SetLineSpace(float);
    float GetCharSpace() const;
    void SetCharSpace(float);
    HorizontalPosition GetTextPositionX() const;
    void SetTextPositionX(HorizontalPosition);
    VerticalPosition GetTextPositionY() const;
    void SetTextPositionY(VerticalPosition);
    TextAlignment GetTextAlignment() const;
    void SetTextAlignment(TextAlignment);
    TagProcessor* GetTagProcessor() const;
    TagProcessorUtf8* GetTagProcessorUtf8() const;

    void SetTagProcessor(TagProcessor* pTagProcessor) {
        if (UpdatePTDirty(m_pTagProcessor.utf16 != pTagProcessor)) {
            m_pTagProcessor.utf16 = pTagProcessor;
        }
    }

    void SetTagProcessorUtf8(TagProcessorUtf8*);
    virtual const util::Unorm8x4 GetVertexColor(int) const;
    virtual void SetVertexColor(int, const util::Unorm8x4&);
    virtual uint8_t GetVertexColorElement(int) const;
    virtual void SetVertexColorElement(int, uint8_t);
    const font::Rectangle GetTextDrawRect() const;
    virtual uint8_t GetMaterialCount() const;
    virtual Material* GetMaterial(int) const;
    void SetMaterial(Material*);
    void UnsetMaterial();
    const char* GetTextId() const;
    bool IsWidthLimitEnabled() const;
    void SetWidthLimitEnabled(bool);
    float GetItalicRatio() const;
    void SetItalicRatio(float);
    float GetAngleFromItalicRatio() const;
    void SetAngleToItalicRatio(float);
    const util::Float2& GetShadowOffset() const;
    void SetShadowOffset(const util::Float2&);
    const util::Float2& GetShadowScale() const;
    void SetShadowScale(const util::Float2&);
    const util::Unorm8x4& GetShadowTopColor() const;
    void SetShadowTopColor(const util::Unorm8x4&);
    const util::Unorm8x4& GetShadowBottomColor() const;
    void SetShadowBottomColor(const util::Unorm8x4&);
    void SetShadowColor(const util::Unorm8x4&, const util::Unorm8x4&);
    float GetShadowItalicRatio() const;
    void SetShadowItalicRatio(float);
    float GetAngleFromShadowItalicRatio() const;
    void SetAngleToShadowItalicRatio(float);
    void SetDirtyFlag(bool);
    void ClearDirtyFlag();
    bool GetDirtyFlag() const;
    virtual void SetupTextWriter(font::WideTextWriter*);
    virtual void SetupTextWriterUtf8(font::TextWriter*);
    void SetPerCharacterTransformOffset(float);
    void SetPerCharacterTransform(int, float);
    float GetPerCharacterTransform(int) const;
    bool GetUtf8() const;
    void SetInvisibleBorderEnabled(bool);
    bool IsInvisibleBorderEnabled() const;
    void SetDoubleDrawnBorderEnabled(bool);
    bool IsDoubleDrawnBorderEnabled() const;
    void SetCenterCeilingEnabled(bool);
    bool IsCenterCeilingEnabled() const;
    void SetDrawFromRightToLeftEnabled(bool);
    bool IsDrawFromRightToLeftEnabled() const;
    font::DispStringBuffer* GetDispStringBuffer() const;
    void GetTextGlobalMtx(util::MatrixT4x3fType*, const DrawInfo&) const;
    virtual void Calculate(DrawInfo&, CalculateContext&, bool);
    bool CompareCopiedInstanceTest(const TextBox&) const;

protected:
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);
    virtual void LoadMtx(DrawInfo&);
    virtual bool InitializeStringWithTextSearcherInfo(gfx::Device*, const BuildArgSet&,
                                                      const TextSearcher::TextInfo&);
    virtual bool InitializeStringWithTextSearcherInfoUtf8(gfx::Device*, const BuildArgSet&,
                                                          const TextSearcher::TextInfoUtf8&);
    void InitializePerCharacterTransformCurves(const ResAnimationInfo*);
    void CopyImpl(const TextBox&, gfx::Device*, ResourceAccessor*, const char*);
    void CopyImpl(const TextBox&, gfx::Device*, uint16_t, ResourceAccessor*, const char*);
    void CopyCommonImpl(const TextBox&);

private:
    void SetFontInfo(font::WideTextWriter*) const;
    void SetFontInfoUtf8(font::TextWriter*) const;
    void SetTextPos(font::WideTextWriter*) const;
    void SetTextPosUtf8(font::TextWriter*) const;
    util::Float2 AdjustTextPos(const Size&, bool) const;
    void Initialize();
    void InitializeMaterial();
    void CopyLineWidthOffset(const TextBox&);
    void CopyPerCharacterTransform(const TextBox&);
    void SetupConstantBufferAdditionalContent(font::ConstantBufferAdditionalContent&) const;
    void BuildConstantBuffer(DrawInfo&) const;
    uint16_t SetStringImpl(const uint16_t*, uint16_t, int);
    uint16_t SetStringImplUtf8(const char*, uint16_t, int);

    bool UpdatePTDirty(bool isChanged) {
        m_Bits.isPTDirty |= isChanged;
        return isChanged;
    }

    bool UpdatePTDirty(const util::Unorm8x4&, const util::Unorm8x4&);
    const TextBox& operator=(const TextBox&);
    float GetLoopFrame(float, float, float) const;

    template <typename T>
    void UpdatePerCharacterTransform(font::TagProcessorBase<T>*);

    template <typename T>
    float CalculateStep(font::TagProcessorBase<T>*);

    template <typename T>
    const void* AcquireNextPrintableChar(uint32_t*, font::TagProcessorBase<T>*, const void*);

    bool ValidateNextPrintableChar(const char*, const char*);
    bool ValidateNextPrintableChar(const uint16_t*, const uint16_t*);
    uint32_t GetCharFromPointer(const char*);
    uint32_t GetCharFromPointer(const uint16_t*);
    void ApplyPerCharacterTransformCurve(font::PerCharacterTransformInfo*, float);

    union {
        uint16_t* utf16;
        char* utf8;
        void* neutral;
    } m_TextBuf;

    const char* m_pTextId;
    util::Unorm8x4 m_TextColors[2];
    const font::Font* m_pFont;
    Size m_FontSize;
    float m_LineSpace;
    float m_CharSpace;

    union {
        TagProcessor* utf16;
        TagProcessorUtf8* utf8;
        void* neutral;
    } m_pTagProcessor;

    uint16_t m_TextBufLen;
    uint16_t m_TextLen;

    struct Bits {
        uint8_t textAlignment : 2;
        uint8_t isPTDirty : 1;
        bool shadowEnabled : 1;
        bool invisibleBorderEnabled : 1;
        bool doubleDrawnBorderEnabled : 1;
        bool widthLimitEnabled : 1;
        bool perCharacterTransformEnabled : 1;
        /* Bits is 1 byte in botw and smo
        bool centerCeilingEnabled : 1;
        bool perCharacterTransformSplitByCharWidth : 1;
        bool perCharacterTransformAutoShadowAlpha : 1;
        bool drawFromRightToLeft : 1;
        bool perCharacterTransformOriginToCenter : 1;
        */
    };

    Bits m_Bits;
    uint8_t _11d;
    uint8_t m_TextPosition;
    bool m_IsUtf8;
    float m_ItalicRatio;
    util::Float2 m_ShadowOffset;
    util::Float2 m_ShadowScale;
    util::Unorm8x4 m_ShadowTopColor;
    util::Unorm8x4 m_ShadowBottomColor;
    float m_ShadowItalicRatio;

    struct LineWidthOffset {
        float* pLineWidth;
        float* pLineOffset;
    };

    LineWidthOffset* m_pLineWidthOffset;
    Material* m_pMaterial;
    font::DispStringBuffer* m_pDispStringBuf;

    struct CurveInfo {
        const ResAnimationTarget* pCurve;
        AnimTargetPerCharacterTransformCurve CurveType;
    };

    struct PerCharacterTransform {
        float Offset;
        float Width;
        font::PerCharacterTransformInfo* pPerCharacterTransformInfos;
        uint8_t LoopType;
        uint8_t OriginV;
        uint8_t CurveNumber;
        uint8_t Dummy;
        float OriginVOffset;
        CurveInfo CurveInfos[1];
    };

    PerCharacterTransform* m_pPerCharacterTransform;
};

}  // namespace ui2d
}  // namespace nn
