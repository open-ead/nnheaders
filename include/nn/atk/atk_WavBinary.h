#pragma once

#include <nn/types.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/fnd/binary/atkfnd_PrimitiveTypes.h>

namespace nn::atk::detail {
struct ChunkHeader {
    ChunkHeader() = default;

    explicit ChunkHeader(u32 id);
    ChunkHeader(u32 id, u32 size);

    u32 id;
    fnd::PcBinU32 size;
};
static_assert(sizeof(ChunkHeader) == 0x8);

struct RiffChunk {
    static const u32 ValidId {1179011410}; // "RIFF"

    RiffChunk() = default;

    bool IsValid();

    ChunkHeader header;
    u32 formatType;
};
static_assert(sizeof(RiffChunk) == 0xc);

struct FmtChunk {
    static const u32 ValidId {0x20746d66}; // "fmt "
    static const u16 FormatPcm {SampleFormat_PcmS16};

    FmtChunk() = default;

    static u32 GetValidBodySize();

    bool IsValid();

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
    static const u32 ValidId {0x61746164}; // "data"
    
    DataChunk() = default;

    bool IsValid();

    ChunkHeader header;
};
static_assert(sizeof(DataChunk) == 0x8);

struct WaveBinaryHeader {
    WaveBinaryHeader() = default;
    
    RiffChunk riffChunk;
    FmtChunk fmtChunk;
    DataChunk dataChunk;
};
static_assert(sizeof(WaveBinaryHeader) == 0x2c);
} // namespace nn::atk::detail