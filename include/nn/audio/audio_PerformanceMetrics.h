#pragma once

#include <nn/types.h>

#include <nn/audio/audio_PerformanceMetricsTypes.h>

namespace nn::audio {
class PerformanceInfo {
public:
    PerformanceInfo& operator=(const PerformanceInfo&);
    PerformanceInfo& operator=(PerformanceInfo&&);

    PerformanceInfo();
    ~PerformanceInfo();

    bool SetBuffer(const void* buffer, size_t bufferSize);

    bool MoveToNextFrame();

    s32 GetTotalProcessingTime();
    u32 GetFrameIndex();
    const PerformanceEntry* GetEntries(int*);
    const PerformanceDetail* GetDetails(int*);

private:
    const void* m_Buffer;
    size_t m_BufferSize;
    PerformanceFrameHeader* m_Header;
    PerformanceEntry* m_Entries;
    PerformanceDetail* m_Details;
};
static_assert(sizeof(PerformanceInfo) == 0x28);
} // namespace nn::audio