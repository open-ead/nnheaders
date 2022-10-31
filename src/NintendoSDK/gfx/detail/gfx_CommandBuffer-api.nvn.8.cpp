#include <nn/gfx/detail/gfx_CommandBuffer-api.nvn.8.h>

#include <nn/gfx/detail/deviceimpl.h>
#include <nn/gfx/detail/gfx_State-api.nvn.8.h>
#include <nn/gfx/memory.h>

#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

size_t CommandBufferImpl<NvnApi>::GetCommandMemoryAlignment(Device* device) {
    int align;
    nvnDeviceGetInteger(device->pnDevice, NVN_DEVICE_INFO_COMMAND_BUFFER_COMMAND_ALIGNMENT, &align);
    return align;
}

size_t CommandBufferImpl<NvnApi>::GetControlMemoryAlignment(Device* device) {
    int align;
    nvnDeviceGetInteger(device->pnDevice, NVN_DEVICE_INFO_COMMAND_BUFFER_CONTROL_ALIGNMENT, &align);
    return align;
}

CommandBufferImpl<NvnApi>::CommandBufferImpl() {}

CommandBufferImpl<NvnApi>::~CommandBufferImpl() {}

void CommandBufferImpl<NvnApi>::Initialize(Device* device, const InfoType& unused) {
    pNnDevice = device;
    pNvnCommandBuffer = &nvnCommandBuffer;

    nvnCommandBufferInitialize(pNvnCommandBuffer, device->pnDevice);

    pOutOfCommandMemoryCallback = nullptr;
    pOutOfControlMemoryCallback = nullptr;
    userPtr = nullptr;

    // todo: figure out what to do with this
    const auto callback = [](NVNcommandBuffer* buffer, NVNcommandBufferMemoryEvent event,
                             size_t size, void* userData) {
        auto pThis = static_cast<CommandBufferImpl*>(userData);
        auto cmdBuf = reinterpret_cast<TCommandBuffer<NvnApi>*>(pThis);
        OutOfMemoryEventArg arg{size};

        switch (event) {
        case NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_CONTROL_MEMORY:
            reinterpret_cast<OutOfMemoryEventCallback>(pThis->pOutOfCommandMemoryCallback.ptr)(
                cmdBuf, arg);
            break;

        case NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_COMMAND_MEMORY:
            reinterpret_cast<OutOfMemoryEventCallback>(pThis->pOutOfCommandMemoryCallback.ptr)(
                cmdBuf, arg);
            break;

        default:
            // nn::detail::UnexpectedDefaultImpl("", "", 0, userData);
            pThis->Finalize();
            break;
        }
    };

    nvnCommandBufferSetMemoryCallback(pNvnCommandBuffer, callback);
    nvnCommandBufferSetMemoryCallbackData(pNvnCommandBuffer, this);

    hNvnCommandBuffer = 0;

    flags.SetBit(Flag_ConservativeRasterSupported,
                 device->deviceFeatures & Device::Feature_SupportsConservativeRaster);
    flags.SetBit(Flag_Shared, false);

    state = State_Initialized;
}

void CommandBufferImpl<NvnApi>::Finalize() {
    if (hNvnCommandBuffer != 0) {
        nvnDeviceFinalizeCommandHandle(pNnDevice->pnDevice, hNvnCommandBuffer);
        hNvnCommandBuffer = 0;
    }

    nvnCommandBufferFinalize(pNvnCommandBuffer);
    state = State_NotInitialized;
}

void CommandBufferImpl<NvnApi>::AddCommandMemory(MemoryPool* pool, ptrdiff_t ptr, size_t size) {
    nvnCommandBufferAddCommandMemory(pNvnCommandBuffer, pool->pnPool, ptr, size);
}

void CommandBufferImpl<NvnApi>::AddControlMemory(void* ptr, size_t size) {
    nvnCommandBufferAddControlMemory(pNvnCommandBuffer, ptr, size);
}

void CommandBufferImpl<NvnApi>::SetOutOfCommandMemoryEventCallback(
    OutOfMemoryEventCallback callback) {
    pOutOfCommandMemoryCallback = reinterpret_cast<void (*)()>(callback);
}

void CommandBufferImpl<NvnApi>::SetOutOfControlMemoryEventCallback(
    OutOfMemoryEventCallback callback) {
    pOutOfControlMemoryCallback = reinterpret_cast<void (*)()>(callback);
}

void CommandBufferImpl<NvnApi>::Reset() {}

void CommandBufferImpl<NvnApi>::Begin() {
    if (hNvnCommandBuffer != 0) {
        nvnDeviceFinalizeCommandHandle(pNnDevice->pnDevice, hNvnCommandBuffer);
        hNvnCommandBuffer = 0;
    }

    nvnCommandBufferBeginRecording(pNvnCommandBuffer);
    state = State_Begun;
}

void CommandBufferImpl<NvnApi>::End() {
    hNvnCommandBuffer = nvnCommandBufferEndRecording(pNvnCommandBuffer);
    state = State_Initialized;
}

void CommandBufferImpl<NvnApi>::Dispatch(int a, int b, int c) {
    nvnCommandBufferDispatchCompute(pNvnCommandBuffer, a, b, c);
}

void CommandBufferImpl<NvnApi>::Draw(PrimitiveTopology top, int a, int b) {
    nvnCommandBufferDrawArrays(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top), a, b);
}

void CommandBufferImpl<NvnApi>::Draw(PrimitiveTopology top, int a, int b, int c, int d) {
    nvnCommandBufferDrawArraysInstanced(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top), a, b, c, d);
}

void CommandBufferImpl<NvnApi>::DrawIndexed(PrimitiveTopology top, IndexFormat index,
                                            const GpuAddress& addr, int a, int b) {
    nvnCommandBufferDrawElementsBaseVertex(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top),
                                           Nvn::GetIndexFormat(index), a,
                                           Nvn::GetBufferAddress(addr), b);
}

void CommandBufferImpl<NvnApi>::DrawIndexed(PrimitiveTopology top, IndexFormat index,
                                            const GpuAddress& addr, int a, int b, int c, int d) {
    nvnCommandBufferDrawElementsInstanced(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top),
                                          Nvn::GetIndexFormat(index), a,
                                          Nvn::GetBufferAddress(addr), b, c, d);
}

void CommandBufferImpl<NvnApi>::DispatchIndirect(const GpuAddress& addr) {
    nvnCommandBufferDispatchComputeIndirect(pNvnCommandBuffer, Nvn::GetBufferAddress(addr));
}

void CommandBufferImpl<NvnApi>::DrawIndirect(PrimitiveTopology top, const GpuAddress& addr) {
    nvnCommandBufferDrawArraysIndirect(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top),
                                       Nvn::GetBufferAddress(addr));
}

void CommandBufferImpl<NvnApi>::DrawIndexedIndirect(PrimitiveTopology top, IndexFormat index,
                                                    const GpuAddress& addr1,
                                                    const GpuAddress& addr2) {
    nvnCommandBufferDrawElementsIndirect(pNvnCommandBuffer, Nvn::GetDrawPrimitive(top),
                                         Nvn::GetIndexFormat(index), Nvn::GetBufferAddress(addr1),
                                         Nvn::GetBufferAddress(addr2));
}

// todo: figure out the PipelineImpl struct
// void CommandBufferImpl<NvnApi>::SetPipeline(const Pipeline*);

void CommandBufferImpl<NvnApi>::SetRasterizerState(const Raster* pRast) {
    const RasterizerStateImplData<NvnApi>& rast = pRast->ToData();

    nvnCommandBufferBindPolygonState(
        pNvnCommandBuffer, reinterpret_cast<const NVNpolygonState*>(&rast.nvnPolygonState));
    nvnCommandBufferSetPolygonOffsetClamp(pNvnCommandBuffer, rast.nvnSlopeScaledDepthBias,
                                          rast.nvnDepthBias, rast.nvnDepthBiasClamp);
    nvnCommandBufferBindMultisampleState(
        pNvnCommandBuffer, reinterpret_cast<const NVNmultisampleState*>(&rast.nvnMultisampleState));

    if (rast.flags.GetBit(rast.Flag_MultisampleEnabled)) {
        nvnCommandBufferSetSampleMask(pNvnCommandBuffer, rast.nvnSampleMask);
    }

    nvnCommandBufferSetDepthClamp(pNvnCommandBuffer,
                                  !rast.flags.GetBit(rast.Flag_DepthClipEnabled));
    nvnCommandBufferSetRasterizerDiscard(pNvnCommandBuffer,
                                         !rast.flags.GetBit(rast.Flag_RasterEnabled));

    if (flags.GetBit(Flag_ConservativeRasterSupported)) {
        nvnCommandBufferSetConservativeRasterEnable(
            pNvnCommandBuffer, rast.flags.GetBit(rast.Flag_ConservativeRasterEnabled));
    }
}

void CommandBufferImpl<NvnApi>::SetBlendState(const BlendStateImpl<NvnApi>* pBlend) {
    const BlendStateImplData<NvnApi>& blend = pBlend->ToData();
    const NVNblendState* pnBlendState = blend.pNvnBlendStateData;

    for (int i = 0; i < blend.targetCount; ++i) {
        nvnCommandBufferBindBlendState(pNvnCommandBuffer, &pnBlendState[i]);
    }

    nvnCommandBufferBindChannelMaskState(
        pNvnCommandBuffer, reinterpret_cast<NVNchannelMaskState*>(blend.nvnChannelMaskState));
    nvnCommandBufferBindColorState(pNvnCommandBuffer,
                                   reinterpret_cast<NVNcolorState*>(blend.nvnColorState));
    nvnCommandBufferSetBlendColor(pNvnCommandBuffer, blend.nvnBlendConstant);
}

void CommandBufferImpl<NvnApi>::SetDepthStencilState(const DepthStencilStateImpl<NvnApi>* depth) {
    nvnCommandBufferBindDepthStencilState(pNvnCommandBuffer, depth->nDepthStencilState);
    nvnCommandBufferSetStencilValueMask(pNvnCommandBuffer, NVN_FACE_FRONT_AND_BACK, depth->field_8);
    nvnCommandBufferSetStencilMask(pNvnCommandBuffer, NVN_FACE_FRONT_AND_BACK, depth->field_C);
    nvnCommandBufferSetStencilRef(pNvnCommandBuffer, NVN_FACE_BACK, depth->field_10);
    nvnCommandBufferSetStencilRef(pNvnCommandBuffer, NVN_FACE_FRONT, depth->field_14);
    if ((depth->flag & 8) == 0)
        nvnCommandBufferSetDepthBounds(pNvnCommandBuffer, false, 0.0, 1.0);
}

}  // namespace nn::gfx::detail