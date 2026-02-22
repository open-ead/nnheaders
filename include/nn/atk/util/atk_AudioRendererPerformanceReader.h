#pragma once

#include <atomic>

#include <nn/types.h>
#include <nn/os.h>

namespace nn::atk {
class AudioRendererPerformanceReader {
public:
    struct PerformanceInfo {
        void* performanceBuffer;
        size_t performanceBufferSize;
        os::Tick tick;
    };

    AudioRendererPerformanceReader();
    
    static size_t GetRequiredMemorySize(s32);

    void Initialize(s32, void*, u64);
    PerformanceInfo* ReadPerformanceInfo();

    void Record(const void* performanceFrameBuffer, size_t performanceFrameBufferSize, os::Tick tick);

private:
    PerformanceInfo* m_pPerformanceInfo{};
    s32 m_PerformanceInfoCount{0};
    std::atomic_int m_WriteIndex{0};
    std::atomic_int m_ReadIndex{0};
    bool m_IsInitialized{false};
};
static_assert(sizeof(AudioRendererPerformanceReader) == 0x18);

} // namespace nn::atk