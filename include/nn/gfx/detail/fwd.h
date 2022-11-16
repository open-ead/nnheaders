#pragma once

namespace nn::gfx {

template <typename TTarget>
class TDevice;

template <typename TTarget>
class TCommandBuffer;

template <typename TTarget>
class TMemoryPool;

template <typename TTarget>
struct BufferImplData;

template <typename TTarget>
struct CommandBufferImplData;

template <typename TTarget>
struct RasterizerStateImplData;

template <typename TTarget>
struct BlendStateImplData;

template <typename TTarget>
struct DepthStencilStateImplData;

template <typename TTarget>
struct VertexStateImplData;

template <typename TTarget>
struct TessellationStateImplData;

template <typename TTarget>
struct ViewportScissorStateImplData;

template <typename TTarget>
struct PipelineImplData;

namespace detail {

template <typename TTarget>
class MemoryPoolImpl;

template <typename TTarget>
class DeviceImpl;

template <typename TTarget>
class BufferImpl;

template <typename TTarget>
class CommandBufferImpl;

template <typename TTarget>
class PipelineImpl;

template <typename TTarget>
class RootSignatureImpl;

template <typename TTarget>
class ShaderImpl;

template <typename TTarget>
class TextureImpl;

template <typename TTarget>
class DescriptorPoolImpl;

template <typename TTarget>
class SamplerImpl;

template <typename TTarget>
class RasterizerStateImpl;

template <typename TTarget>
class BlendStateImpl;

template <typename TTarget>
class DepthStencilStateImpl;

template <typename TTarget>
class VertexStateImpl;

template <typename TTarget>
class TessellationStateImpl;

template <typename TTarget>
class ViewportScissorStateImpl;

template <typename TTarget>
class ColorTargetViewImpl;

template <typename TTarget>
class DepthStencilViewImpl;

template <typename TTarget>
class TextureViewImpl;

}  // namespace detail

}  // namespace nn::gfx