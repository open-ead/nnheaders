#pragma once

#include <nn/types.h>

#include <nn/atk/fnd/basis/atkfnd_PrimitiveTypes.h>

namespace nn::atk::detail {
struct ChunkHeader {
    u32 id;
    fnd::PcBinU32 size;
};
static_assert(sizeof(ChunkHeader) == 0x8);

struct RiffChunk {
    ChunkHeader header;
    u32 formatType;
};
static_assert(sizeof(RiffChunk) == 0xc);

struct FmtChunk {
    ChunkHeader header;
    fnd::PcBinU16 formatTag;
    fnd::PcBinU16 channels;
    fnd::PcBinU32 samplesPerSec;
    fnd::PcBinU32 avgBytesPerSec;
    fnd::PcBinU16 blockAlign;
    fnd::PcBinU16 bitsPerSample;
};
static_assert(sizeof(FmtChunk) == 0x18);

struct DataChunk {
    ChunkHeader header;
};
static_assert(sizeof(DataChunk) == 0x8);

struct WaveBinaryHeader {
    RiffChunk riffChunk;
    FmtChunk fmtChunk;
    DataChunk dataChunk;
};
static_assert(sizeof(WaveBinaryHeader) == 0x2c);
} // namespace nn::atk::detail