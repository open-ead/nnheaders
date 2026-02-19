#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
struct BinaryFileHeader {
    s32 signature;
    u16 byteOrder;
    u16 headerSize;
    u32 version;
    u32 fileSize;
    u16 dataBlocks;
    u16 reserved;
};
static_assert(sizeof(BinaryFileHeader) == 0x14);

struct BinaryBlockHeader {
    // Named "kind", but functionally the same as BinaryFileHeader.signature
    s32 kind;
    u32 size;
};
static_assert(sizeof(BinaryBlockHeader) == 0x8);

} // namespace nn::atk::detail