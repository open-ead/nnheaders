#pragma once

#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {
class WavOutFileStream {
public:
    constexpr static u8 FileIoBufferAlignment = 8;

    WavOutFileStream();
    ~WavOutFileStream();

    bool Open(fnd::FileStream& stream, s32, u64);
    
    bool WriteHeader(s32, u64);

    void Close();

    bool FlushBuffer();

    u64 CalcRiffChunkSize(u64);

    bool UpdateRiffChunkSize();
    bool UpdateDataChunkSize();

    std::size_t Write(const void* buffer, std::size_t size);
    std::size_t WriteDirect(const void* buf, std::size_t length, fnd::FndResult* result);

    bool Seek(position_t offset, fnd::Stream::SeekOrigin origin);

    void SetCacheBuffer(char* cacheBuffer, std::size_t cacheBufferSize);

private:
    fnd::FileStream* m_pFileStream;
    std::size_t m_WaveDataSize;
    bool m_IsWaveDataSizeCalculating;
    char* m_Buffer;
    std::size_t m_BufferLength;
    std::size_t m_ValidBufferLength;
};
} // namespace nn::atk::detail