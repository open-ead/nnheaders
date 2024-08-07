#pragma once

#include <nn/font/font_ResourceFormat.h>
#include <nn/types.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/util/util_MathTypes.h>

namespace nn {
namespace ui2d {

template <size_t N>
constexpr uint32_t MakeSig(const char (&s)[N]) {
    static_assert(N == 5, "signature must have exactly 4 characters");
    return s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24);
}

const uint32_t BinaryFileFormatVersion = 0x8060000;
const uint32_t BinaryCpxFileFormatVersion = 0x9030000;
const uint32_t FileSignatureFlyt = MakeSig("FLYT");
const uint32_t DataBlockKindLyt = MakeSig("lyt1");
const uint32_t DataBlockKindControl = MakeSig("cnt1");
const uint32_t DataBlockKindTextureList = MakeSig("txl1");
const uint32_t DataBlockKindFontList = MakeSig("fnl1");
const uint32_t DataBlockKindMateriaList = MakeSig("mat1");
const uint32_t DataBlockKindPane = MakeSig("pan1");
const uint32_t DataBlockKindPicture = MakeSig("pic1");
const uint32_t DataBlockKindTextBox = MakeSig("txt1");
const uint32_t DataBlockKindWindow = MakeSig("wnd1");
const uint32_t DataBlockKindBounding = MakeSig("bnd1");
const uint32_t DataBlockKindParts = MakeSig("prt1");
const uint32_t DataBlockKindGroup = MakeSig("grp1");
const uint32_t DataBlockKindCapture = MakeSig("cpt1");
const uint32_t DataBlockKindUserDataList = MakeSig("usd1");
const uint32_t DataBlockKindPaneStart = MakeSig("pas1");
const uint32_t DataBlockKindPaneEnd = MakeSig("pae1");
const uint32_t DataBlockKindGroupStart = MakeSig("grs1");
const uint32_t DataBlockKindGroupEnd = MakeSig("gre1");
const uint32_t FileSignatureFlan = MakeSig("FLAN");
const uint32_t BinFileSignatureFcpx = MakeSig("FCPX");

const uint32_t DataBlockKindAnimTag = MakeSig("pat1");
const uint32_t DataBlockKindAnimShare = MakeSig("pah1");
const uint32_t DataBlockKindAnim = MakeSig("pai1");

const uint32_t ResourceTypeLayout = 'blyt';
const uint32_t ResourceTypeAnimation = 'anim';
const uint32_t ResourceTypeTexture = 'timg';
const uint32_t ResourceTypeFont = 'font';
const uint32_t ResourceTypeShader = 'bgsh';
const uint32_t ResourceTypeScalableFont = 'scft';

struct ResVec2 {
    void Set(float, float);
    operator util::Float2() const;

    float x;
    float y;
};

struct ResVec3 {
    void Set(float, float, float);
    operator util::Vector3fType() const;

    float x;
    float y;
    float z;
};

struct ResTexSrt {
    ResVec2 translate;
    float rotate;
    ResVec2 scale;
};

struct ResProjectionTexGenParameters {
    ResVec2 translate;
    ResVec2 scale;
    uint8_t flag;
    uint8_t reserve[3];
};

class ResTexCoordGen {
public:
    ResTexCoordGen();
    ResTexCoordGen(TexGenType, TexGenSrc);
    void Set(TexGenType, TexGenSrc);
    TexGenType GetTexGenType() const;
    TexGenSrc GetTexGenSrc() const;
    bool IsProjection() const;
    bool IsPerspectiveProjection() const;
    void SetProjectionTexGenParameters(const ResProjectionTexGenParameters*);
    void SetProjectionTexGenParamaters(const ResProjectionTexGenParameters*);

private:
    uint8_t m_TexGenType;
    uint8_t m_TexGenSrc;
    char padding[2];
    uint64_t m_AddressOfProjectionTexGenParams;
};

class ResTevStage {
public:
    ResTevStage();
    void Set(TevMode, TevMode);
    void SetCombineRgb(TevMode);
    void SetCombineAlpha(TevMode);
    TevMode GetCombineRgb() const;
    TevMode GetCombineAlpha() const;

private:
    uint8_t m_CombineRgb;
    uint8_t m_CombineAlpha;
    char padding[2];
};

class ResAlphaCompare {
public:
    ResAlphaCompare();
    ResAlphaCompare(AlphaTest aFunc, float aRef) {
        m_AlphaCompareFunc = aFunc;
        m_AlphaRef = aRef;
    }

    void Set(AlphaTest, float);
    void SetRef(float);
    AlphaTest GetFunc() const { return (AlphaTest)m_AlphaCompareFunc; }
    float GetRef() const;

private:
    uint8_t m_AlphaCompareFunc;
    float m_AlphaRef;
};

class ResBlendMode {
public:
    ResBlendMode();
    ResBlendMode(BlendOp, BlendFactor, BlendFactor, LogicOp);
    void Set(BlendOp, BlendFactor, BlendFactor, LogicOp);
    bool IsBlendOpDisable() const;
    BlendOp GetBlendOp() const;
    gfx::BlendFunction GetGfxBlendOp() const;
    BlendFactor GetSrcFactor() const;
    gfx::BlendFactor GetGfxSrcFactor() const;
    BlendFactor GetDstFactor() const;
    gfx::BlendFactor GetGfxDstFactor() const;
    bool IsLogicOpDisable() const;
    LogicOp GetLogicOp() const;
    gfx::LogicOperation GetGfxLogicOp() const;
    bool Equals(const ResBlendMode*) const;

private:
    gfx::BlendFactor ConvertBlendFactor(BlendFactor) const;

    uint8_t m_BlendOperation;
    uint8_t m_SrcBlendFactor;
    uint8_t m_DestBlendFactor;
    uint8_t m_LogicOp;
};

class ResIndirectParameter {
public:
    float rotate;
    ResVec2 scale;

    ResIndirectParameter();
};

class ResFontShadowParameter {
public:
    uint8_t blackInterporateColor[3];
    uint8_t whiteInterporateColor[4];
    uint8_t reserve;

    ResFontShadowParameter();
};

struct ResWindowInflation {
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
};

struct ResWindowFrameSize {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
};

// 955:    nn::ui2d::ResSystemExtUserData;

class ResExtUserData {
public:
    ResExtUserData(uint32_t, uint32_t, uint16_t, uint8_t);

    const char* GetName() const {
        if (GetNameOffset() != 0) {
            return util::ConstBytePtr(this, GetNameOffset()).Get<char>();
        }

        return nullptr;
    }

    ExtUserDataType GetType() const { return static_cast<ExtUserDataType>(m_Type); }
    uint16_t GetCount() const;
    const char* GetString() const;
    const int32_t* GetIntArray() const;
    const float* GetFloatArray() const;
    int GetSystemDataVersion() const;
    int GetSystemDataCount() const;
    const void* GetSystemData(int) const;

    void WriteIntValue(int32_t value, int index) {
        auto pValues = util::BytePtr(this, GetDataOffset()).Get<int32_t>();
        pValues[index] = value;
    }

    void WriteFloatValue(float value, int index) {
        auto pValues = util::BytePtr(this, GetDataOffset()).Get<float>();
        pValues[index] = value;
    }

    uint32_t GetDataOffset() const { return m_DataOffset; }
    uint32_t GetNameOffset() const { return m_NameStrOffset; }

private:
    uint32_t m_NameStrOffset;
    uint32_t m_DataOffset;
    uint16_t m_Count;
    uint8_t m_Type;
    char m_Padding1;
};

struct ResAnimationGroupRef {
    char name[33];
    uint8_t flag;
    char padding[2];

    void SetDefault();

    const char* GetName() const { return name; }
};

struct ResAnimationShareInfo {
    char srcPaneName[25];
    char targetGroupName[25];
    char padding[2];

    void SetDefault();

    const char* GetSrcPaneName() const { return srcPaneName; }
    const char* GetTargetGroupName() const { return targetGroupName; }
};

struct ResLayout {
    font::detail::BinaryBlockHeader blockHeader;
    uint8_t originType;
    char padding[3];
    ResVec2 layoutSize;
    ResVec2 partsSize;
};

struct ResControl {
    font::detail::BinaryBlockHeader blockHeader;
    uint32_t controlUserNameOffset;
    uint32_t controlFunctionalPaneNamesOffset;
    uint16_t controlFunctionalPaneCount;
    uint16_t controlFunctionalAnimCount;
    uint32_t controlFunctionalPaneParameterNameOffsetsOffset;
    uint32_t controlFunctionalAnimParameterNameOffsetsOffset;
};

/*
1424:   nn::ui2d::ResFont;
1448:   nn::ui2d::ResFontList;
1471:   nn::ui2d::ResTexture;
1495:   nn::ui2d::ResTextureList;
*/
struct ResTexMap {
    uint16_t texIdx;
    uint8_t wrapSflt;
    uint8_t wrapTflt;

    void SetDefault();
    TexWrap GetWarpModeS() const;
    TexWrap GetWarpModeT() const;
    TexFilter GetMinFilter() const;
    TexFilter GetMagFilter() const;
    void SetWarpModeS(uint8_t);
    void SetWarpModeT(uint8_t);
    void SetMinFilter(uint8_t);
    void SetMagFilter(uint8_t);
};

/*
1627:   nn::ui2d::ResTexMapAdditionalInfo;
1631:   nn::ui2d::ResTexMapAdditionalInfo::InfoType;
1648:   nn::ui2d::ResMaterialResourceCount;
*/

struct ResColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    operator util::Unorm8x4() const;
};

/*
2012:   nn::ui2d::ResMaterial;
2054:   nn::ui2d::ResMaterialList;
2074:   nn::ui2d::ResShapeInfoGfxPrimitiveRoundRect;
2085:   nn::ui2d::ResShapeInfoGfxPrimitiveCircle;
2095:   nn::ui2d::ResShapeInfo;
2134:   nn::ui2d::ResShapeInfoList;
2155:   nn::ui2d::ResCaptureTextureOld;
2182:   nn::ui2d::ResCaptureTexture;
2214:   nn::ui2d::ResCaptureTextureFilter;
2243:   nn::ui2d::ResCaptureTextureList;
*/

struct ResPane {
    font::detail::BinaryBlockHeader blockHeader;
    uint8_t flag;
    uint8_t basePosition;
    uint8_t alpha;
    uint8_t flagEx;
    char name[24];
    char userData[8];
    ResVec3 translate;
    ResVec3 rotate;
    ResVec2 scale;
    ResVec2 size;
};

struct ResPicture : public ResPane {
    ResColor vtxCols[4];
    uint16_t materialIdx;
    uint8_t texCoordCount;
    uint8_t flags;
};

// not sure where to place this and seems to downcast in nn::ui2d::Bounding
struct ResBounding : public ResPane {};

struct ResPerCharacterTransform {
    float evalTimeOffset;
    float evalTimeWidth;
    uint8_t loopType;
    uint8_t originV;
    uint8_t hasAnimationInfo;
    char padding[1];
};

/*
2413:   nn::ui2d::ResPerCharacterTransformExtended;
*/

struct ResTextBox : public ResPane {
    uint16_t textBufBytes;
    uint16_t textStrBytes;
    uint16_t materialIdx;
    uint16_t fontIdx;
    uint8_t textPosition;
    uint8_t textAlignment;
    uint16_t textBoxFlag;
    float italicRatio;
    uint32_t textStrOffset;
    ResColor textCols[2];
    ResVec2 fontSize;
    float charSpace;
    float lineSpace;
    uint32_t textIdOffset;
    ResVec2 shadowOffset;
    ResVec2 shadowScale;
    ResColor shadowCols[2];
    float shadowItalicRatio;
    uint32_t lineWidthOffsetOffset;
    uint32_t perCharacterTransformOffset;
};

/*
2508:   nn::ui2d::ResWindowFrame;
2529:   nn::ui2d::ResWindowContent;
*/

struct ResWindow : public ResPane {
    ResWindowInflation inflation;
    ResWindowFrameSize frameSize;
    uint8_t frameCount;
    uint8_t windowFlags;
    char padding[2];
    uint32_t contentOffset;
    uint32_t frameOffsetTableOffset;
};

struct ResPartsPaneBasicInfo {
    char userData[8];
    ResVec3 translate;
    ResVec3 rotate;
    ResVec2 scale;
    ResVec2 size;
    uint8_t alpha;
    char padding[3];
};

struct ResPartsProperty {
    char name[24];
    uint8_t usageFlag;
    uint8_t basicUsageFlag;
    uint8_t materialUsageFlag;
    char padding[1];
    uint32_t propertyOffset;
    uint32_t extUserDataOffset;
    uint32_t paneBasicInfoOffset;
};

struct ResParts : public ResPane {
    uint32_t propertyCount;
    ResVec2 magnify;
};

struct ResExtUserDataList {
    font::detail::BinaryBlockHeader blockHeader;
    uint16_t count;
    char padding[2];
};

/*
2740:   nn::ui2d::ResCapture;
2751:   nn::ui2d::ResAlignment;
*/

struct ResGroup {
    font::detail::BinaryBlockHeader blockHeader;
    char name[33];
    char padding[1];
    uint16_t paneCount;
};

struct ResDetailedCombinerStage {
    enum Src {
        Src_Texture0 = 3,
        Src_Texture1 = 4,
        Src_Texture2 = 5,
        Src_Texture3 = 6,
        Src_Constant = 14,
        Src_Primary = 0,
        Src_Previous = 15,
        Src_PreviousBuffer = 13,
        Src_Num = 8
    };

    enum OperandRgb {
        OperandRgb_Rgb,
        OperandRgb_InverseRgb,
        OperandRgb_Alpha,
        OperandRgb_InverseAlpha,
        OperandRgb_Rrr,
        OperandRgb_InverseRrr,
        OperandRgb_Ggg = 8,
        OperandRgb_InverseGgg,
        OperandRgb_Bbb = 12,
        OperandRgb_InverseBbb,
        OperandRgb_Num = 10
    };

    enum OperandAlpha {
        OperandAlpha_Alpha,
        OperandAlpha_InverseAlpha,
        OperandAlpha_R,
        OperandAlpha_InverseR,
        OperandAlpha_G,
        OperandAlpha_InverseG,
        OperandAlpha_B,
        OperandAlpha_InverseB,
        OperandAlpha_Num
    };

    enum Combine {
        Combine_Replace,
        Combine_Modulate,
        Combine_Add,
        Combine_AddSigned,
        Combine_Interpolate,
        Combine_Subtract,
        Combine_Dot3Rgb,
        Combine_Dot3Rgba,
        Combine_AddMult,
        Combine_MultAdd,
        Combine_Num
    };

    enum ConstColorSelect {
        ConstColorSelect_Black,
        ConstColorSelect_White,
        ConstColorSelect_C0,
        ConstColorSelect_C1,
        ConstColorSelect_C2,
        ConstColorSelect_C3,
        ConstColorSelect_C4,
        ConstColorSelect_C5,
        ConstColorSelect_Max
    };

    enum Scale { Scale_1, Scale_2, Scale_4, Scale_NUM };

    void Set(Combine, Combine, Src, Src, Src, Src, Src, Src, OperandRgb, OperandRgb, OperandRgb,
             OperandAlpha, OperandAlpha, OperandAlpha, Scale, Scale, ConstColorSelect,
             ConstColorSelect, bool, bool);
    void SetCombineRgb(Combine);
    void SetCombineAlpha(Combine);
    void SetSrcRgb0(Src);
    void SetSrcRgb1(Src);
    void SetSrcRgb2(Src);
    void SetSrcAlpha0(Src);
    void SetSrcAlpha1(Src);
    void SetSrcAlpha2(Src);
    void SetOperandRgb0(OperandRgb);
    void SetOperandRgb1(OperandRgb);
    void SetOperandRgb2(OperandRgb);
    void SetOperandAlpha0(OperandAlpha);
    void SetOperandAlpha1(OperandAlpha);
    void SetOperandAlpha2(OperandAlpha);
    void SetScaleRgb(Scale);
    void SetScaleAlpha(Scale);
    void SetConstColorSelectRgb(ConstColorSelect);
    void SetConstColorSelectAlpha(ConstColorSelect);
    void SetSavePrevRgb(bool);
    void SetSavePrevAlpha(bool);
    void SetCombineRgbSourceCount(Combine);
    void SetCombineAlphaSourceCount(Combine);
    Combine GetCombineRgb() const;
    Combine GetCombineAlpha() const;
    Src GetSrcRgb0() const;
    Src GetSrcRgb1() const;
    Src GetSrcRgb2() const;
    Src GetSrcAlpha0() const;
    Src GetSrcAlpha1() const;
    Src GetSrcAlpha2() const;
    OperandRgb GetOperandRgb0() const;
    OperandRgb GetOperandRgb1() const;
    OperandRgb GetOperandRgb2() const;
    OperandAlpha GetOperandAlpha0() const;
    OperandAlpha GetOperandAlpha1() const;
    OperandAlpha GetOperandAlpha2() const;
    Scale GetScaleRgb() const;
    Scale GetScaleAlpha() const;
    ConstColorSelect GetConstColorSelectRgb() const;
    ConstColorSelect GetConstColSelAlpha() const;
    bool GetSavePrevRgb() const;
    bool GetSavePrevAlpha() const;
    uint32_t GetCombineRgbSourceCount() const;
    uint32_t GetCombineAlphaSourceCount() const;
    void SetDefault();

    Bit32 bits0;
    Bit32 bits1;
    Bit32 bits2;
    Bit32 bits3;

    enum Bits {
        Bits_CombineRgb = 4,
        Bits_CombineAlpha = 4,
        Bits_SrcRgb = 4,
        Bits_SrcAlpha = 4,
        Bits_OperandRgb = 4,
        Bits_OperandAlpha = 4,
        Bits_Scale = 2,
        Bits_ConstColorSelect = 4,
        Bits_SavePrev = 1,
        Bits_CombineRgb_Count = 4,
        Bits_CombineAlpha_Count = 4
    };

    enum Pos3 { Pos3_Rgb_Count = 0, Pos3_Alpha_Count = 4, Field3_Size = 8 };
};

struct ResDetailedCombinerStageInfo {
public:
    void SetDefault();

    void SetConstantColor(uint32_t idx, ResColor color) { m_ConstColor[idx] = color; }

    ResColor GetConstantColor(uint32_t idx) const { return m_ConstColor[idx % 5]; }

    void SetBufferColorSelectRgb(ResDetailedCombinerStage::ConstColorSelect);
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectRgb() const;
    void SetBufferColorSelectAlpha(ResDetailedCombinerStage::ConstColorSelect);
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectAlpha() const;

private:
    Bit32 m_Bits;
    ResColor m_ConstColor[6];
};

/*
3743:   nn::ui2d::ResConstantColor;
*/

struct ResCombinerUserShader {
public:
    void SetConstantColor(uint32_t, ResColor);
    ResColor GetConstantColor(uint32_t) const;
    const uint32_t* GetKeyCode() const;
    void SetKeyCode(uint32_t*, size_t);

private:
    uint32_t m_KeyCode[24];
    ResColor m_ConstColor[5];
};

struct ResAnimationTagBlock {
    font::detail::BinaryBlockHeader blockHeader;
    uint16_t tagOrder;
    uint16_t groupCount;
    uint32_t nameOffset;
    uint32_t groupsOffset;
    uint32_t userDataListOffset;
    int16_t startFrame;
    int16_t endFrame;
    uint8_t flag;
    char padding[3];
};

struct ResAnimationShareBlock {
    font::detail::BinaryBlockHeader blockHeader;
    uint32_t animShareInfoOffset;
    uint16_t shareCount;
    char padding[2];
};

struct ResAnimationBlock {
    font::detail::BinaryBlockHeader blockHeader;
    uint16_t frameSize;
    uint8_t loop;
    char padding[1];
    uint16_t fileCount;
    uint16_t animContCount;
    uint32_t animContOffsetsOffset;
};

struct ResAnimationContent {
    char name[28];
    uint8_t count;
    uint8_t type;
    char padding[2];

    /*
    uint32_t animInfoOffsets;
    uint32_t fileNameOffsets;
    */
};

struct ResAnimationInfo {
    uint32_t kind;
    uint8_t count;
    char padding[3];
};

struct ResAnimationTarget {
    uint8_t id;
    uint8_t target;
    uint8_t curveType;
    char padding[1];
    uint16_t keyCount;
    char padding2[2];
    uint32_t keysOffset;
};

struct ResHermiteKey {
    float frame;
    float value;
    float slope;
};

struct ResStepKey {
    float frame;
    uint16_t value;
    uint16_t padding[1];
};

struct ResParameterizedAnim {
    uint16_t parameterizedAnimCount;
    char padding[2];
};

/*
4202:   nn::ui2d::ResParameterizedAnimParameter;
4222:   nn::ui2d::SystemDataSimpleAABB;
4240:   nn::ui2d::SystemDataSimpleOBB;
4260:   nn::ui2d::SystemDataAlignmentExInfo;
4264:   nn::ui2d::SystemDataAlignmentExInfo::AlignmentFlags;
4289:   nn::ui2d::SystemDataMaskTexture;
*/

struct CaptureTextureCopyInfo {
    uint8_t useFlags;
    uint8_t targetId;
    uint8_t padding[2];
    char names[3][64];
};

/*
4338:   nn::ui2d::SystemDataCopyCaptureTextureInfo;
4355:   nn::ui2d::SystemDataMaskTextureRuntimeInfo;
*/
}  // namespace ui2d
}  // namespace nn
