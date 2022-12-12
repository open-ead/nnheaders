// this file's name isn't confirmed, following the naming structure anyways

#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nvn/nvn.h>

namespace nn::gfx {

template <>
class TInteroperation<NvnApi> {
    static void ConvertToGfxDevice(TDevice<NvnApi>*, NVNdevice*);
    static void ConvertToGfxQueue(TQueue<NvnApi>*, NVNqueue*, TDevice<NvnApi>*);
    static void ConvertToGfxMemoryPool(TMemoryPool<NvnApi>*, NVNmemoryPool*, void*);
    static void ConvertToGfxDescriptorPool(TDescriptorPool<NvnApi>*, NVNtexturePool*);
    static void ConvertToGfxDescriptorPool(TDescriptorPool<NvnApi>*, NVNsamplerPool*);
    static void ConvertToGfxBuffer(TBuffer<NvnApi>*, NVNbuffer*);
    static void ConvertToGfxCommandBuffer(TCommandBuffer<NvnApi>*, TDevice<NvnApi>*,
                                          NVNcommandBuffer*);
    static void ConvertToGfxSampler(TSampler<NvnApi>*, NVNsampler*);
    static void ConvertToGfxShader(TShader<NvnApi>*, NVNprogram*, bool, int);
    static void ConvertToGfxTexture(TTexture<NvnApi>*, NVNtexture*);
    static void ConvertToGfxTextureView(TTextureView<NvnApi>*, NVNtexture*, NVNtextureView*);
    static void ConvertToGfxColorTargetView(TColorTargetView<NvnApi>*, NVNtexture*,
                                            NVNtextureView*);
    static void ConvertToGfxDepthStencilView(TDepthStencilView<NvnApi>*, NVNtexture*,
                                             NVNtextureView*);
    static NVNformat ConvertToNvnFormat(ImageFormat);
    static NVNformat ConvertToNvnFormat(AttributeFormat);
};

}  // namespace nn::gfx