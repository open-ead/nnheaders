#pragma once

#include <nn/font/font_Font.h>
#include <nn/font/font_GlyphTreeMap.h>
#include <nn/font/font_Types.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Texture.h>
#include <nn/gfx/gfx_Types.h>

namespace nn {

namespace fontll {
class ScalableFontEngine;
class OtfKerningTable;
}  // namespace fontll

namespace font {

class FontMetrics;

// todo: reorder
class TextureCache {
    struct LineInfo {
        GlyphLineList list;
        uint16_t y;
        uint16_t currentX;
        uint8_t kind;
        uint8_t no;
        uint8_t dummy[2];
    };

    class LineHeight;

    enum NoSpaceError {
        NoSpaceError_NoError,
        NoSpaceError_LackOfGlyphNodeSpace,
        NoSpaceError_TextureCacheLineIsOverMax,
        NoSpaceError_LackOfTextureCacheSpace
    };

public:
    static const int FontFaceCountMax;
    static const int MultiScalableFontCountMax;
    static const int CharCodeRangeCountMax;

private:
    static const int FontNameLengthMax;
    static const int WorkMemorySizeDefault;
    static const int WorkMemorySizeNoPlotDefault;
    static const int GlyphNodeCountMaxDefault;
    static const int TextureCacheLineCountMax;
    static const int GlyphPadding;
    static const int CoreCountMax;
    fontll::ScalableFontEngine* m_pFontEngine;
    fontll::ScalableFontEngine* m_pFontEngineNoPlot;
    char* m_pFontNameBuffers;
    uint8_t* m_pTextureBitmap;
    uint32_t m_TextureCacheWidth;
    uint32_t m_TextureCacheHeight;
    GlyphTreeMap m_GlyphTreeMap;
    GlyphList m_NeedPlotGlyphList;
    GlyphList m_NeedEraseGlyphList;
    GlyphList m_NotInFontGlyphList;
    LineInfo m_LineInfos[256];
    uint32_t m_LineCurrentPos;
    uint32_t m_FontCount;
    uint8_t m_FontListCount[32];
    uint8_t* m_InnerFontFaceHeadTable[32];
    uint8_t m_InnerFontFaceTable[512];
    uint32_t m_CurrentFontFace;
    uint32_t m_CurrentFontFacesNoPlot[8];
    NoSpaceError m_NoSpaceError;
    bool m_IsWorkMemoryExhausted;
    bool m_IsDrawMultiCore;
    int m_CoreCount;

    int (*m_pGetCoreNumberFunction)();
    uint8_t (*m_pCalculateLineKindFunction)(uint16_t);
    uint32_t (*m_pCalculateLineHeightFunction)(uint8_t);

    FontMetrics* m_pFontMetrics;
    int32_t* m_pBoldWeights;
    uint8_t* m_pBorderWidths;
    bool* m_pDeleteBearingX;
    int16_t* m_pBearingOffsetX;
    int* m_pCharCodeRangeCount;

    uint32_t (*m_pCharCodeRangeFirst)[16];
    uint32_t (*m_pCharCodeRangeLast)[16];

    fontll::OtfKerningTable** m_pOtfKerningTable;
    bool m_IsExactWidthReturnedAfterPlot;
    gfx::Texture m_Texture;
    gfx::TextureView m_TextureView;
    gfx::MemoryPool m_MemoryPoolForTexture;
    gfx::MemoryPool* m_pActiveMemoryPoolForTexture;
    ptrdiff_t m_OffsetOfMemoryPoolForTexture;
    PlacementTextureObject m_TexObj;

public:
    TextureCache();
    virtual ~TextureCache();

    struct InitializeArg;

    static void SetMemoryPoolInfo(gfx::MemoryPoolInfo*);
    static size_t CalculateMemoryPoolAlignment(gfx::Device*, int, int);
    static size_t CalculateMemoryPoolSize(gfx::Device*, int, int);
    void Initialize(gfx::Device*, const InitializeArg&);
    void Initialize(gfx::Device*, const InitializeArg&, gfx::MemoryPool*, ptrdiff_t, size_t);
    void Finalize(gfx::Device*, FreeFunctionWithUserData, void*);
    void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot, void*);
    void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot, void*);
    bool RegisterGlyph(uint32_t, uint32_t, uint16_t, int);
    bool RegisterGlyph(uint32_t, uint32_t, uint16_t, int, bool);
    uint32_t RegisterGlyphsWithLength(const uint16_t*, uint32_t, uint32_t, uint16_t, int);
    uint32_t RegisterGlyphsWithLength(const uint16_t*, uint32_t, uint32_t, uint16_t, int, bool);
    uint32_t RegisterGlyphsWithLengthUtf8(const char*, uint32_t, uint32_t, uint16_t, int);
    uint32_t RegisterGlyphsWithLengthUtf8(const char*, uint32_t, uint32_t, uint16_t, int, bool);
    void UpdateTextureCache();
    void CompleteTextureCache();
    void ClearLockAllGlyphs(int);
    void ClearLockGlyphsWithLength(const uint16_t*, uint32_t, uint32_t, uint16_t, int);
    void ClearLockGlyphsWithLength(const uint16_t*, uint32_t, uint32_t, uint16_t, int, bool);
    void ClearLockGlyphsWithLengthUtf8(const char*, uint32_t, uint32_t, uint16_t, int);
    void ClearLockGlyphsWithLengthUtf8(const char*, uint32_t, uint32_t, uint16_t, int, bool);
    void ResetTextureCache();
    bool CreateNewLine(LineHeight);
    void ChangeFontListOrder(uint32_t, uint32_t*);
    TextureObject* GetTextureObject();
    const TextureObject* GetTextureObject() const;
    uint32_t GetTextureCacheWidth() const;
    uint32_t GetTextureCacheHeight() const;
    bool IsBorderEffectEnabled(uint16_t) const;
    uint32_t CountPlottingGlyph(const uint16_t*, uint32_t, uint32_t, uint16_t);
    void DumpGlyphTreeMap() const;
    bool IsGlyphExistInFont(uint32_t, uint16_t);
    bool IsDrawMultiCore() const;
    NoSpaceError GetNoSpaceError() const;
    void ClearNoSpaceError();
    bool GetWorkMemoryExhausted() const;
    void ClearWorkMemoryExhausted();
    const FontMetrics& GetFontMetrics(uint16_t) const;
    int CalculateCharWidth(uint32_t, uint32_t, uint16_t);
    int CalculateKerning(uint32_t, uint32_t, uint32_t, uint16_t);
    GlyphNode* FindGlyphNode(uint32_t, uint32_t, uint16_t);
    const void* GetTextureCacheBitMap() const;
    uint32_t CountUnusableGlyph(const uint16_t*, uint32_t, uint32_t, uint16_t);

private:
    LineInfo* CreateNewLineImpl(uint8_t);
    GlyphNode* FindAndReserveEraseGlyph(uint8_t, uint16_t);
    void SetFontFace(uint32_t);
    void SetFontFaceNoPlot(uint32_t, uint32_t);
    void AssertFsError(const char*) const;
    void AssertFsErrorNoPlot(const char*, uint32_t) const;
    uint32_t EraseNotInFontGlyphs();
    char* GetFontNameBuf(uint32_t) const;
    uint32_t GetFontEngineNoPlotCount() const;
    uint32_t GetCoreId() const;
    int InitializeFontFaceTable(const InitializeArg&);
    bool GetInnerFontFace(uint32_t*, uint32_t, uint32_t);
    bool CheckCharCodeRange(uint32_t, uint32_t) const;
    bool IsInternalMemoryPoolUsed() const;
};
}  // namespace font
}  // namespace nn
