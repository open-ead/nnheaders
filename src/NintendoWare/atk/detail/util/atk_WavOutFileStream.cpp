#include <nn/atk/atk_WavOutFileStream.h>

#include <new>

#include <nn/atk/atk_WavBinary.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

WavOutFileStream::WavOutFileStream() = default;

WavOutFileStream::~WavOutFileStream() = default;

// NON_MATCHING: requires WavOutFileStream::WriteHeader
bool WavOutFileStream::Open(fnd::FileStream& stream, int channels, size_t samplesPerSec) {
    m_pFileStream = &stream;

    if (WriteHeader(channels, samplesPerSec)) {
        m_WaveDataSize = 0;
        m_IsWaveDataSizeCalculating = true;
        return true;
    }

    return false;
}

// NON_MATCHING: reordering of instructions
bool WavOutFileStream::WriteHeader(int channels, size_t samplesPerSec) {
    char buffer[sizeof(WaveBinaryHeader) + FileIoBufferAlignment];
    void* alignedBuffer {util::BytePtr(buffer).AlignUp(FileIoBufferAlignment).Get()};

    WaveBinaryHeader* header {new (alignedBuffer) WaveBinaryHeader};

    header->riffChunk.formatType = 0x45564157; // "WAVE"
    header->riffChunk.header.size = sizeof(WaveBinaryHeader) - sizeof(ChunkHeader);
    header->fmtChunk.bitsPerSample = 16;
    header->fmtChunk.blockAlign = channels * 2;
    header->fmtChunk.samplesPerSec = samplesPerSec;
    header->fmtChunk.avgBytesPerSec = header->fmtChunk.blockAlign * header->fmtChunk.samplesPerSec;
    header->fmtChunk.channels = channels;
    header->fmtChunk.formatTag = FmtChunk::FormatPcm;

    size_t result {Write(header, sizeof(WaveBinaryHeader))};
    return result == sizeof(WaveBinaryHeader);
}
}  // namespace nn::atk::detail