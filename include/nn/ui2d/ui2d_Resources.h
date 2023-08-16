#pragma once

#include <nn/types.h>

namespace nn {
namespace ui2d {

const uint32_t BinaryFileFormatVersion = 0x8060000;
const uint32_t BinaryCpxFileFormatVersion = 0x9030000;
const uint32_t FileSignatureFlyt = 0x54594c46;          // "FLYT"
const uint32_t DataBlockKindParts = 0x31747270;         // "prt1"
const uint32_t DataBlockKindCapture = 0x31747063;       // "cpt1"
const uint32_t DataBlockKindUserDataList = 0x31647375;  // "usd1"
const uint32_t FileSignatureFlan = 0x4e414c46;          // "FLAN"
const uint32_t BinFileSignatureFcpx = 0x58504346;       // "FCPX"
const uint32_t ResourceTypeLayout = 'blyt';
const uint32_t ResourceTypeAnimation = 'anim';
const uint32_t ResourceTypeTexture = 'timg';
const uint32_t ResourceTypeFont = 'font';
const uint32_t ResourceTypeShader = 'bgsh';
const uint32_t ResourceTypeScalableFont = 'scft';

}  // namespace ui2d
}  // namespace nn
