#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_CommandBufferData-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>

namespace nn::gfx {

class CommandBufferInfo;
class GpuAddress;

namespace detail {

template <>
class CommandBufferImpl<NvnApi> : public DataContainer<CommandBufferImplData<NvnApi>> {
public:
    using Device = DeviceImpl<NvnApi>;
    using MemoryPool = MemoryPoolImpl<NvnApi>;
    using Pipeline = PipelineImpl<NvnApi>;
    using Raster = RasterizerStateImpl<NvnApi>;

    typedef CommandBufferInfo InfoType;

    typedef void (*OutOfMemoryEventCallback)(TCommandBuffer<NvnApi>*, const OutOfMemoryEventArg&);

    static size_t GetCommandMemoryAlignment(Device* device);
    static size_t GetControlMemoryAlignment(Device* device);

    CommandBufferImpl();
    ~CommandBufferImpl();

    void Initialize(Device* device, const InfoType& unused);
    void Finalize();

    void AddCommandMemory(MemoryPool* pool, ptrdiff_t ptr, size_t size);
    void AddControlMemory(void* ptr, size_t size);
    void SetOutOfCommandMemoryEventCallback(OutOfMemoryEventCallback callback);
    void SetOutOfControlMemoryEventCallback(OutOfMemoryEventCallback callback);

    void Reset();
    void Begin();
    void End();

    void Dispatch(int a, int b, int c);

    void Draw(PrimitiveTopology top, int a, int b);
    void Draw(PrimitiveTopology top, int a, int b, int c, int d);
    void DrawIndexed(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr, int a,
                     int b);
    void DrawIndexed(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr, int a, int b,
                     int c, int d);
    void DispatchIndirect(const GpuAddress& addr);
    void DrawIndirect(PrimitiveTopology top, const GpuAddress& addr);
    void DrawIndexedIndirect(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr1,
                             const GpuAddress& addr2);

    // todo: figure out the PipelineImpl struct
    void SetPipeline(const Pipeline*);
    void SetRasterizerState(const Raster* rast);
    void SetBlendState(const BlendStateImpl<NvnApi>* blend);
    void SetDepthStencilState(const DepthStencilStateImpl<NvnApi>* depth);
};

}  // namespace detail

}  // namespace nn::gfx
