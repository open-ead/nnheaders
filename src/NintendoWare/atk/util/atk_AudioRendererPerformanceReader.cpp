#include <nn/atk/atk_AudioRendererPerformanceReader.h>
#include <nn/atk/atk_SoundSystem.h>

namespace nn::atk {
AudioRendererPerformanceReader::AudioRendererPerformanceReader() = default;

size_t AudioRendererPerformanceReader::GetRequiredMemorySize(int performanceInfoCount) {
    size_t size {performanceInfoCount * 
                (SoundSystem::GetPerformanceFrameBufferSize() + sizeof(PerformanceInfo))};
    return size;
}
} // namespace nn::atk