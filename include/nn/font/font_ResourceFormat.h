#pragma once

#include <nn/types.h>

namespace nn {
namespace font {
namespace detail {

typedef union {
    uint32_t UInt32;
    int32_t SInt32;
    float Float32;
} Type32;

struct BinaryFileHeader {
    uint32_t signature;
    uint16_t byteOrder;
    uint16_t headerSize;
    uint32_t version;
    uint32_t fileSize;
    uint16_t dataBlocks;
    uint16_t reserved;
};

struct BinaryBlockHeader {
    uint32_t kind;
    uint32_t size;
};

inline bool IsValidBinaryFile(const BinaryFileHeader* pHeader, uint32_t signature, uint32_t version,
                              uint16_t minBlocks) {
    NN_UNUSED(version);
    NN_UNUSED(minBlocks);

    if (pHeader->signature != signature) {
        char s[256];
        util::SNPrintf(s, sizeof(s), "Signature check failed ('%c%c%c%c' must be '%c%c%c%c').",
                       (pHeader->signature >> 24) & 0xff, (pHeader->signature >> 16) & 0xff,
                       (pHeader->signature >> 8) & 0xff, pHeader->signature & 0xff,
                       (signature >> 24) & 0xff, (signature >> 16) & 0xff, (signature >> 8) & 0xff,
                       signature & 0xff);

        return false;
    }

    return true;
}
}  // namespace detail

/*
406 : nn::font::FontType;
417 : nn::font::CharacterCode;
431 : nn::font::FontMapMethod;
445 : nn::font::FontSheetFormat;
485 : nn::font::CharWidths;
498 : nn::font::CMapScanEntry;
511 : nn::font::CMapInfoScan;
523 : nn::font::KerningFirstTableElem;
533 : nn::font::KerningSecondTableElem;
544 : nn::font::KerningSecondTable;
594 : nn::font::FontTextureGlyph;
616 : nn::font::FontWidth;
631 : nn::font::FontCodeMap;
647 : nn::font::FontKerningTable;
667 : nn::font::FontInformation;
687 : nn::font::ResSubFontType;
695 : nn::font::ResCharCodeRange;
702 : nn::font::ResCharCodeRangeSet;
710 : nn::font::ResBitmapFont;
718 : nn::font::ResScalableFontDescription;
726 : nn::font::ResScalableFontDescription::Bits;
743 : nn::font::ResScalableFontDescriptionOld;
754 : nn::font::ResMultiScalableFont;
769 : nn::font::ResMultiScalableFontOld;
779 : nn::font::ResPairFont;
787 : nn::font::ResComplexFontDescription;
*/
}  // namespace font
}  // namespace nn
