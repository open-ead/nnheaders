#pragma once

#include <atomic>

#include <nn/os.h>

namespace nn::atk {
class AudioRendererPerformanceReader {
public:
    static const int PerformanceInfoCountMin = 2;

    struct PerformanceInfo {
        void* performanceBuffer;
        size_t performanceBufferSize;
        os::Tick tick;
    };

    AudioRendererPerformanceReader();
    
    static size_t GetRequiredMemorySize(int performanceInfoCount);

    void Initialize(int performanceInfoCount, void* buffer, size_t bufferSize);
    const PerformanceInfo* ReadPerformanceInfo();

    bool IsInitialized() const;

    void Record(const void* performanceFrameBuffer, size_t performanceFrameBufferSize, os::Tick tick);

private:
    PerformanceInfo* m_pPerformanceInfo{};
    int m_PerformanceInfoCount {0};
    std::atomic_int m_WriteIndex {0};
    std::atomic_int m_ReadIndex {0};
    bool m_IsInitialized {false};
};
static_assert(sizeof(AudioRendererPerformanceReader) == 0x18);
} // namespace nn::atk