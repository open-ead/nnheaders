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
class RootSignatureImpl;

}  // namespace detail

}  // namespace nn::gfx