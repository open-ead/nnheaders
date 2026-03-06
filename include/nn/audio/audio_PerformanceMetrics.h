#pragma once

#include <nn/types.h>

#include <nn/audio/audio_PerformanceMetricsTypes.h>

namespace nn::audio {
class PerformanceInfo {
public:
    PerformanceInfo();
    ~PerformanceInfo();

    void SetBuffer(const void* buffer, size_t bufferSize);
    void MoveToNextFrame();

    PerformanceEntry* GetEntries(s32*);
    void GetTotalProcessingTime();

private:
    void* m_Buffer;
    size_t m_BufferSize;
    PerformanceFrameHeader* m_Header;
    PerformanceEntry* m_Entries;
    PerformanceDetail* m_Details;
};
static_assert(sizeof(PerformanceInfo) == 0x28);
} // namespace nn::audio