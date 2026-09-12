#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {

class WavOutFileStream {
public:
    static const int FileIoBufferAlignment = alignof(char);

    WavOutFileStream();
    ~WavOutFileStream();

    bool Open(fnd::FileStream& stream, int channels, size_t samplesPerSec);
    void Close();

    size_t Write(const void* buf, size_t length);
    bool Seek(position_t offset, fnd::FileStream::SeekOrigin origin);

    bool IsAvailable() const { return m_pFileStream != nullptr && m_pFileStream->IsOpened(); }

    size_t GetSize() const;

    position_t Tell() const;

    void SetCacheBuffer(char* buf, size_t length);

private:
    bool WriteHeader(int channels, size_t samplesPerSec);

    bool UpdateRiffChunkSize();
    bool UpdateDataChunkSize();
    size_t CalcRiffChunkSize(size_t dataSize);

    size_t WriteDirect(const void* buf, size_t length, fnd::FndResult* result);

    size_t FlushBuffer();

    fnd::FileStream* m_pFileStream{};
    size_t m_WaveDataSize{0};
    bool m_IsWaveDataSizeCalculating{false};
    char* m_Buffer{};
    size_t m_BufferLength{0};
    size_t m_ValidBufferLength{0};
};
static_assert(sizeof(WavOutFileStream) == 0x30);

}  // namespace nn::atk::detail