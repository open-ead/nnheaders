#pragma once

namespace nn::gfx {

template <typename TTarget>
class TDevice;

template <typename TTarget>
class TCommandBuffer;

template <typename TTarget>
class TMemoryPool;

template <typename TTarget>
class TTexture;

template <typename TTarget>
class TPipeline;

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

template <typename TTarget>
struct ShaderImplData;

template <typename TTarget>
struct TextureImplData;

template <typename TTarget>
struct TextureViewImplData;

template <typename TTarget>
struct ColorTargetViewImplData;

template <typename TTarget>
struct DepthStencilViewImplData;

template <typename TTarget>
struct MemoryPoolImplData;

template <typename TTarget>
struct DeviceImplData;

template <typename TTarget>
struct DescriptorPoolImplData;

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

template <typename TTarget>
class BufferTextureViewImpl;

}  // namespace detail

}  // namespace nn::gfx