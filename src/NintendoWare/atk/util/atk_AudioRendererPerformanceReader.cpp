#include <nn/atk/atk_AudioRendererPerformanceReader.h>
#include <nn/atk/atk_SoundSystem.h>

namespace nn::atk {
AudioRendererPerformanceReader::AudioRendererPerformanceReader() = default;

size_t AudioRendererPerformanceReader::GetRequiredMemorySize(int performanceInfoCount) {
    size_t size {performanceInfoCount * 
                (SoundSystem::GetPerformanceFrameBufferSize() + sizeof(PerformanceInfo))};
    return size;
}

void AudioRendererPerformanceReader::Initialize(int performanceInfoCount, void* buffer, size_t bufferSize) {
    m_PerformanceInfoCount = performanceInfoCount;
    
    auto ptr {util::BytePtr(buffer)};
    m_pPerformanceInfo = ptr.Get<PerformanceInfo>();

    ptr += performanceInfoCount * sizeof(PerformanceInfo);
    
    const size_t performanceBufferSize {SoundSystem::GetPerformanceFrameBufferSize()};
    for (int i {0}; i < m_PerformanceInfoCount; ++i) {
        m_pPerformanceInfo[i].performanceBuffer = ptr.Get();
        m_pPerformanceInfo[i].performanceBufferSize = performanceBufferSize;
        ptr += performanceBufferSize;
    }

    m_WriteIndex = 0;
    m_ReadIndex = m_PerformanceInfoCount - 1;
    m_IsInitialized = true;
}
} // namespace nn::atk