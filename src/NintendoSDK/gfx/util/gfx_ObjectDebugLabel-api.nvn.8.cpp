#include <nn/gfx/gfx_Common.h>

#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_Texture.h>

#include <nvn/nvn_FuncPtrInline.h>

namespace nn::gfx::util {
template <>
void SetMemoryPoolDebugLabel<NvnApi>(TMemoryPool<NvnApi>* pMemoryPool, const char* label) {
    nvnMemoryPoolSetDebugLabel(pMemoryPool->ToData()->pNvnMemoryPool, label);
}

template <>
void SetBufferDebugLabel<NvnApi>(TBuffer<NvnApi>* pBuffer, const char* label) {
    nvnBufferSetDebugLabel(pBuffer->ToData()->pNvnBuffer, label);
}

template <>
void SetTextureDebugLabel<NvnApi>(TTexture<NvnApi>* pTexture, const char* label) {
    nvnTextureSetDebugLabel(pTexture->ToData()->pNvnTexture, label);
}

template <>
void SetSamplerDebugLabel<NvnApi>(TSampler<NvnApi>* pSampler, const char* label) {
    nvnSamplerSetDebugLabel(pSampler->ToData()->pNvnSampler, label);
}

}  // namespace nn::gfx::util