#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_CommandBufferData-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>

namespace nn::gfx {

class CommandBufferInfo;
class GpuAddress;
class TextureSubresource;
class TextureCopyRegion;
class BufferTextureCopyRegion;
class TextureArrayRange;
class TextureSubresourceRange;
class DescriptorSlot;
class ViewportStateInfo;
class ScissorStateInfo;

namespace detail {

template <>
class CommandBufferImpl<NvnApi> : public DataContainer<CommandBufferImplData<NvnApi>> {
    NN_GFX_NO_COPY(CommandBufferImpl);

public:
    typedef CommandBufferInfo InfoType;

    typedef void (*OutOfMemoryEventCallback)(TCommandBuffer<NvnApi>*, const OutOfMemoryEventArg&);

    static size_t GetCommandMemoryAlignment(DeviceImpl<NvnApi>*);
    static size_t GetControlMemoryAlignment(DeviceImpl<NvnApi>*);

    CommandBufferImpl();
    ~CommandBufferImpl();

    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);

    void AddCommandMemory(MemoryPoolImpl<NvnApi>*, ptrdiff_t, size_t);
    void AddControlMemory(void*, size_t);

    void SetOutOfCommandMemoryEventCallback(OutOfMemoryEventCallback);
    void SetOutOfControlMemoryEventCallback(OutOfMemoryEventCallback);

    void Reset();
    void Begin();
    void End();
    void Dispatch(int, int, int);

    void Draw(PrimitiveTopology, int, int);
    void Draw(PrimitiveTopology, int, int, int, int);
    void DrawIndexed(PrimitiveTopology, IndexFormat, const GpuAddress&, int, int);
    void DrawIndexed(PrimitiveTopology, IndexFormat, const GpuAddress&, int, int, int, int);
    void DispatchIndirect(const GpuAddress&);
    void DrawIndirect(PrimitiveTopology, const GpuAddress&);
    void DrawIndexedIndirect(PrimitiveTopology, IndexFormat, const GpuAddress&, const GpuAddress&);

    void SetPipeline(const PipelineImpl<NvnApi>*);
    void SetRenderTargets(int, const ColorTargetViewImpl<NvnApi>* const*,
                          const DepthStencilViewImpl<NvnApi>*);
    void SetVertexBuffer(int, const GpuAddress&, ptrdiff_t, size_t);
    void SetViewportScissorState(const ViewportScissorStateImpl<NvnApi>*);

    void CopyBuffer(BufferImpl<NvnApi>*, ptrdiff_t, const BufferImpl<NvnApi>*, ptrdiff_t, size_t);
    void CopyImage(TextureImpl<NvnApi>*, const TextureSubresource&, int, int, int,
                   const TextureImpl<NvnApi>*, const TextureCopyRegion&);
    void CopyBufferToImage(TextureImpl<NvnApi>*, const BufferImpl<NvnApi>*,
                           const BufferTextureCopyRegion&);
    void CopyBufferToImage(TextureImpl<NvnApi>*, const TextureCopyRegion&,
                           const BufferImpl<NvnApi>*, ptrdiff_t);
    void CopyImageToBuffer(BufferImpl<NvnApi>*, const TextureImpl<NvnApi>*,
                           const BufferTextureCopyRegion&);
    void CopyImageToBuffer(BufferImpl<NvnApi>*, ptrdiff_t, const TextureImpl<NvnApi>*,
                           const TextureCopyRegion&);
    void BlitImage(TextureImpl<NvnApi>*, const TextureCopyRegion&, const TextureImpl<NvnApi>*,
                   const TextureCopyRegion&, int);
    void ClearBuffer(BufferImpl<NvnApi>*, ptrdiff_t, size_t, uint32_t);
    void ClearColor(ColorTargetViewImpl<NvnApi>*, float, float, float, float,
                    const TextureArrayRange*);
    void ClearColorTarget(ColorTargetViewImpl<NvnApi>*, const ClearColorValue&,
                          const TextureArrayRange*);
    void ClearDepthStencil(DepthStencilViewImpl<NvnApi>*, float, int, DepthStencilClearMode,
                           const TextureArrayRange*);

    void Resolve(TextureImpl<NvnApi>*, int, int, const ColorTargetViewImpl<NvnApi>*,
                 const TextureArrayRange*);
    void FlushMemory(int);
    void InvalidateMemory(int);

    void CallCommandBuffer(const CommandBufferImpl<NvnApi>*);
    void CopyCommandBuffer(const CommandBufferImpl<NvnApi>*);

    void SetBufferStateTransition(BufferImpl<NvnApi>*, int, int, int, int);
    void SetTextureStateTransition(TextureImpl<NvnApi>*, const TextureSubresourceRange*, int, int,
                                   int, int);
    void SetDescriptorPool(const DescriptorPoolImpl<NvnApi>*);
    void SetRootSignature(PipelineType, RootSignatureImpl<NvnApi>*);
    void SetRootBufferDescriptorTable(PipelineType, int, const DescriptorSlot&);
    void SetRootTextureAndSamplerDescriptorTable(PipelineType, int, const DescriptorSlot&,
                                                 const DescriptorSlot&);
    void SetRootConstantBuffer(PipelineType, int, const GpuAddress&, size_t);
    void SetRootUnorderedAccessBuffer(PipelineType, int, const GpuAddress&, size_t);
    void SetRootTextureAndSampler(PipelineType, int, const TextureViewImpl<NvnApi>*,
                                  const SamplerImpl<NvnApi>*);

    void BeginQuery(QueryTarget);
    void EndQuery(const GpuAddress&, QueryTarget);
    void WriteTimestamp(const GpuAddress&);

    void SetDepthBounds(float, float);
    void SetLineWidth(float);
    void SetViewports(int, int, const ViewportStateInfo*);
    void SetScissors(int, int, const ScissorStateInfo*);

    void SetConstantBuffer(int, ShaderStage, const DescriptorSlot&);
    void SetUnorderedAccessBuffer(int, ShaderStage, const DescriptorSlot&);
    void SetTextureAndSampler(int, ShaderStage, const DescriptorSlot&, const DescriptorSlot&);
    void SetTexture(int, ShaderStage, const DescriptorSlot&);
    void SetImage(int, ShaderStage, const DescriptorSlot&);

    void SetConstantBuffer(int, ShaderStage, const GpuAddress&, size_t);
    void SetUnorderedAccessBuffer(int, ShaderStage, const GpuAddress&, size_t);
    void SetTextureAndSampler(int, ShaderStage, const TextureViewImpl<NvnApi>*,
                              const SamplerImpl<NvnApi>*);
    void SetImage(int, ShaderStage, const TextureViewImpl<NvnApi>*);

    void SetShader(const ShaderImpl<NvnApi>*, int);
    void SetRasterizerState(const RasterizerStateImpl<NvnApi>*);
    void SetBlendState(const BlendStateImpl<NvnApi>*);
    void SetDepthStencilState(const DepthStencilStateImpl<NvnApi>*);
    void SetVertexState(const VertexStateImpl<NvnApi>*);
    void SetTessellationState(const TessellationStateImpl<NvnApi>*);
};

}  // namespace detail

}  // namespace nn::gfx
