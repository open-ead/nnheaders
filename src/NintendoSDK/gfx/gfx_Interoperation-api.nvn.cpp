#include <nn/gfx/gfx_Interoperation-api.nvn.8.h>

#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_DescriptorPool.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Queue.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_Shader.h>
#include <nn/gfx/gfx_Texture.h>

#include "detail/gfx_CommonHelper.h"
#include "detail/gfx_NvnHelper.h"

namespace nn::gfx {

void TInteroperation<NvnApi>::ConvertToGfxDevice(TDevice<NvnApi>* pGfxDevice,
                                                 NVNdevice* pNvnDevice) {
    TDevice<NvnApi>::DataType& obj = pGfxDevice->ToData();
    detail::UseMiddleWare();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnDevice = pNvnDevice;
    obj.supportedFeatures = detail::Nvn::GetDeviceFeature(pNvnDevice);
}

void TInteroperation<NvnApi>::ConvertToGfxQueue(TQueue<NvnApi>* pGfxQueue, NVNqueue* pNvnQueue,
                                                TDevice<NvnApi>* pDevice) {
    TQueue<NvnApi>::DataType& obj = pGfxQueue->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnQueue = pNvnQueue;
    obj.pNnDevice = pDevice;
    obj.pImpl = nullptr;
}

void TInteroperation<NvnApi>::ConvertToGfxMemoryPool(TMemoryPool<NvnApi>* pGfxMemoryPool,
                                                     NVNmemoryPool* pNvnMemoryPool, void* pMemory) {
    TMemoryPool<NvnApi>::DataType& obj = pGfxMemoryPool->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnMemoryPool = pNvnMemoryPool;
    obj.pMemory = pMemory;
}

void TInteroperation<NvnApi>::ConvertToGfxDescriptorPool(
    TDescriptorPool<NvnApi>* pGfxDescriptorPool, NVNtexturePool* pNvnTexturePool) {
    TDescriptorPool<NvnApi>::DataType& obj = pGfxDescriptorPool->ToData();
    obj.descriptorPoolType = DescriptorPoolType_TextureView;
    obj.slotCount = nvnTexturePoolGetSize(pNvnTexturePool);
    obj.pDescriptorPool = pNvnTexturePool;
    obj.reservedSlots = 256;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.state = obj.State_Initialized;
}

void TInteroperation<NvnApi>::ConvertToGfxDescriptorPool(
    TDescriptorPool<NvnApi>* pGfxDescriptorPool, NVNsamplerPool* pNvnSamplerPool) {
    TDescriptorPool<NvnApi>::DataType& obj = pGfxDescriptorPool->ToData();
    obj.descriptorPoolType = DescriptorPoolType_Sampler;
    obj.slotCount = nvnSamplerPoolGetSize(pNvnSamplerPool);
    obj.pDescriptorPool = pNvnSamplerPool;
    obj.reservedSlots = 256;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.state = obj.State_Initialized;
}

void TInteroperation<NvnApi>::ConvertToGfxBuffer(TBuffer<NvnApi>* pGfxBuffer,
                                                 NVNbuffer* pNvnBuffer) {
    TBuffer<NvnApi>::DataType& obj = pGfxBuffer->ToData();
    NVNmemoryPoolFlags memoryPoolFlags = nvnMemoryPoolGetFlags(nvnBufferGetMemoryPool(pNvnBuffer));
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_CpuCached, memoryPoolFlags & NVN_MEMORY_POOL_FLAGS_CPU_CACHED);
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnBuffer = pNvnBuffer;
}

void TInteroperation<NvnApi>::ConvertToGfxCommandBuffer(TCommandBuffer<NvnApi>* pGfxCommandBuffer,
                                                        TDevice<NvnApi>* pDevice,
                                                        NVNcommandBuffer* pNvnCommandBuffer) {
    TCommandBuffer<NvnApi>::DataType& obj = pGfxCommandBuffer->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNnDevice = pDevice;
    obj.pNvnCommandBuffer = pNvnCommandBuffer;
}

void TInteroperation<NvnApi>::ConvertToGfxSampler(TSampler<NvnApi>* pGfxSampler,
                                                  NVNsampler* pNvnSampler) {
    TSampler<NvnApi>::DataType& obj = pGfxSampler->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnSampler = pNvnSampler;
}

void TInteroperation<NvnApi>::ConvertToGfxShader(TShader<NvnApi>* pGfxShader,
                                                 NVNprogram* pNvnProgram, bool enableSeperation,
                                                 int shaderBits) {
    TShader<NvnApi>::DataType& obj = pGfxShader->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.flags.SetBit(obj.Flag_SeparationEnable, enableSeperation);
    obj.nvnShaderStageBits = (shaderBits & ShaderStageBit_Compute) ?
                                 NVN_SHADER_STAGE_COMPUTE_BIT :
                                 NVN_SHADER_STAGE_ALL_GRAPHICS_BITS;
    obj.pNvnProgram = pNvnProgram;
    obj.pReflection = nullptr;
}

void TInteroperation<NvnApi>::ConvertToGfxTexture(TTexture<NvnApi>* pGfxTarget,
                                                  NVNtexture* pNvnTexture) {
    TTexture<NvnApi>::DataType& obj = pGfxTarget->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnTexture = pNvnTexture;
}

void TInteroperation<NvnApi>::ConvertToGfxTextureView(TTextureView<NvnApi>* pGfxTargetView,
                                                      NVNtexture* pNvnTexture,
                                                      NVNtextureView* pNvnTextureView) {
    TTextureView<NvnApi>::DataType& obj = pGfxTargetView->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnTexture = pNvnTexture;
    obj.pNvnTextureView = pNvnTextureView;
}

void TInteroperation<NvnApi>::ConvertToGfxColorTargetView(TColorTargetView<NvnApi>* pGfxTargetView,
                                                          NVNtexture* pNvnTexture,
                                                          NVNtextureView* pNvnTextureView) {
    TColorTargetView<NvnApi>::DataType& obj = pGfxTargetView->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnTexture = pNvnTexture;
    obj.pNvnTextureView = pNvnTextureView;
}

void TInteroperation<NvnApi>::ConvertToGfxDepthStencilView(
    TDepthStencilView<NvnApi>* pGfxDepthStencilView, NVNtexture* pNvnTexture,
    NVNtextureView* pNvnTextureView) {
    TDepthStencilView<NvnApi>::DataType& obj = pGfxDepthStencilView->ToData();
    obj.state = obj.State_Initialized;
    obj.flags.SetBit(obj.Flag_Shared, true);
    obj.pNvnTexture = pNvnTexture;
    obj.pNvnTextureView = pNvnTextureView;
}

NVNformat TInteroperation<NvnApi>::ConvertToNvnFormat(ImageFormat format) {
    return detail::Nvn::GetImageFormat(format);
}

NVNformat TInteroperation<NvnApi>::ConvertToNvnFormat(AttributeFormat format) {
    return detail::Nvn::GetAttributeFormat(format);
}

}  // namespace nn::gfx