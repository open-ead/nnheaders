#include <nn/gfx/detail/gfx_Buffer-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/gfx/gfx_BufferInfo.h>

#include <algorithm>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

namespace {

void SetupTextureBuilder(NVNtextureBuilder* pBuilder, NVNdevice* pDevice,
                         const BufferTextureViewInfo& info) {
    const BufferImpl<NvnApi>* pBuffer = info.GetBufferPtr();
    NVNbuffer* pNvnBuffer = pBuffer->ToData()->pNvnBuffer;
    NVNformat nvnFormat = Nvn::GetImageFormat(info.GetImageFormat());

    NVNmemoryPool* pNvnMemoryPool;
    ptrdiff_t bufferOffset = 0;

    pNvnMemoryPool = nvnBufferGetMemoryPool(pNvnBuffer);
    bufferOffset = nvnBufferGetMemoryOffset(pNvnBuffer);

    nvnTextureBuilderSetDevice(pBuilder, pDevice);
    nvnTextureBuilderSetDefaults(pBuilder);
    nvnTextureBuilderSetWidth(pBuilder, info.GetSize());
    nvnTextureBuilderSetHeight(pBuilder, 1);
    nvnTextureBuilderSetDepth(pBuilder, 1);
    nvnTextureBuilderSetFormat(pBuilder, nvnFormat);
    nvnTextureBuilderSetTarget(pBuilder, NVN_TEXTURE_TARGET_BUFFER);
    nvnTextureBuilderSetFlags(pBuilder, NVN_TEXTURE_FLAGS_IMAGE);
    nvnTextureBuilderSetStorage(pBuilder, pNvnMemoryPool, info.GetOffset() + bufferOffset);
}
}  // namespace

size_t BufferImpl<NvnApi>::GetBufferAlignment(DeviceImpl<NvnApi>* pDevice, const BufferInfo& info) {
    int gpuAccessFlag = info.GetGpuAccessFlags();
    int alignment = 8;

    if (gpuAccessFlag & GpuAccess_ConstantBuffer) {
        int uniformBufferAlignment;
        nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice, NVN_DEVICE_INFO_UNIFORM_BUFFER_ALIGNMENT,
                            &uniformBufferAlignment);
        alignment = std::max(alignment, uniformBufferAlignment);
    }

    if (gpuAccessFlag & GpuAccess_IndirectBuffer) {
        int indirectBufferAlignment;
        nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice, NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT,
                            &indirectBufferAlignment);
        alignment = std::max(alignment, indirectBufferAlignment);

        nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice,
                            NVN_DEVICE_INFO_INDIRECT_DISPATCH_ALIGNMENT, &indirectBufferAlignment);
        alignment = std::max(alignment, indirectBufferAlignment);
    }

    if (gpuAccessFlag & GpuAccess_QueryBuffer) {
        int counterAlignment;
        nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice, NVN_DEVICE_INFO_COUNTER_ALIGNMENT,
                            &counterAlignment);
        alignment = std::max(alignment, counterAlignment);
    }

    if (gpuAccessFlag & GpuAccess_UnorderedAccessBuffer) {
        alignment = std::max(alignment, 32);
    }

    if (gpuAccessFlag & (GpuAccess_DepthStencil | GpuAccess_ColorBuffer | GpuAccess_Texture)) {
        alignment = std::max(alignment, 512);
    }

    return alignment;
}

BufferImpl<NvnApi>::BufferImpl() {}
BufferImpl<NvnApi>::~BufferImpl() {}

void BufferImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* pDevice, const BufferInfo& info,
                                    MemoryPoolImpl<NvnApi>* pMemoryPool, ptrdiff_t memoryPoolOffset,
                                    size_t memoryPoolSize) {
    NVNbufferBuilder builder;

    nvnBufferBuilderSetDefaults(&builder);
    nvnBufferBuilderSetDevice(&builder, pDevice->ToData()->pNvnDevice);
    nvnBufferBuilderSetStorage(&builder, pMemoryPool->ToData()->pNvnMemoryPool, memoryPoolOffset,
                               info.GetSize());

    pNvnBuffer = &nvnBuffer;

    NVNboolean isBufferOk = nvnBufferInitialize(pNvnBuffer, &builder);

    int nvnMemoryPoolFlags = nvnMemoryPoolGetFlags(pMemoryPool->ToData()->pNvnMemoryPool);

    flags.SetBit(Flag_CpuCached, nvnMemoryPoolFlags & NVN_MEMORY_POOL_FLAGS_CPU_CACHED);
    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void BufferImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    nvnBufferFinalize(pNvnBuffer);
    pNvnBuffer = nullptr;
    state = State_NotInitialized;
}

void* BufferImpl<NvnApi>::Map() const {
    return nvnBufferMap(pNvnBuffer);
}

void BufferImpl<NvnApi>::Unmap() const {}

void BufferImpl<NvnApi>::FlushMappedRange(ptrdiff_t offset, size_t flushSize) const {
    if (flags.GetBit(Flag_CpuCached)) {
        nvnBufferFlushMappedRange(pNvnBuffer, offset, flushSize);
    }
}

void BufferImpl<NvnApi>::InvalidateMappedRange(ptrdiff_t offset, size_t invalidateSize) const {
    if (flags.GetBit(Flag_CpuCached)) {
        nvnBufferInvalidateMappedRange(pNvnBuffer, offset, invalidateSize);
    }
}

void BufferImpl<NvnApi>::GetGpuAddress(GpuAddress* pOutGpuAddress) const {
    pOutGpuAddress->ToData()->value = nvnBufferGetAddress(pNvnBuffer);
    pOutGpuAddress->ToData()->impl = 0;
}

size_t BufferTextureViewImpl<NvnApi>::GetOffsetAlignment(DeviceImpl<NvnApi>* pDevice,
                                                         const BufferTextureViewInfo& info) {
    NVNtextureBuilder builder;
    SetupTextureBuilder(&builder, pDevice->ToData()->pNvnDevice, info);

    size_t alignment = 0;
    alignment = nvnTextureBuilderGetStorageAlignment(&builder);
    return alignment;
}

BufferTextureViewImpl<NvnApi>::BufferTextureViewImpl() {}
BufferTextureViewImpl<NvnApi>::~BufferTextureViewImpl() {}

void BufferTextureViewImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* pDevice,
                                               const BufferTextureViewInfo& info) {
    pNvnTexture = &nvnTexture;

    NVNtextureBuilder builder;
    SetupTextureBuilder(&builder, pDevice->ToData()->pNvnDevice, info);

    NVNboolean result = nvnTextureInitialize(pNvnTexture, &builder);

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void BufferTextureViewImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    nvnTextureFinalize(pNvnTexture);
    pNvnTexture = nullptr;
    state = State_NotInitialized;
}

}  // namespace nn::gfx::detail