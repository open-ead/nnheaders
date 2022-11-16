#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/gfx_StateData-api.nvn.8.h>
#include <nn/gfx/gfx_StateInfo.h>

namespace nn::gfx {
namespace detail {

template <>
class RasterizerStateImpl<NvnApi> : public DataContainer<RasterizerStateImplData<NvnApi>> {
public:
    typedef RasterizerStateInfo InfoType;

    RasterizerStateImpl();
    ~RasterizerStateImpl();

    void Initialize(DeviceImpl<NvnApi>* device, const InfoType& info);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class BlendStateImpl<NvnApi> : public DataContainer<BlendStateImplData<NvnApi>> {
public:
    typedef BlendStateInfo InfoType;

    static size_t GetRequiredMemorySize(const InfoType& info);

    BlendStateImpl();
    ~BlendStateImpl();

    void SetMemory(void* p, size_t s);
    void* GetMemory();
    void* GetMemory() const;

    void Initialize(DeviceImpl<NvnApi>* device, const InfoType& info);
    void Finalize(DeviceImpl<NvnApi>* device);
};

template <>
class DepthStencilStateImpl<NvnApi> : public DataContainer<DepthStencilStateImplData<NvnApi>> {
public:
    typedef DepthStencilStateInfo InfoType;

    DepthStencilStateImpl();
    ~DepthStencilStateImpl();

    void Initialize(DeviceImpl<NvnApi>*, const DepthStencilStateInfo& info);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class VertexStateImpl<NvnApi> : public DataContainer<VertexStateImplData<NvnApi>> {
public:
    typedef VertexStateInfo InfoType;

    enum RequiredMemoryInfo { RequiredMemoryInfo_Alignment = 8 };

    VertexStateImpl();
    ~VertexStateImpl();
    static size_t GetRequiredMemorySize(const InfoType&);
    void SetMemory(void*, size_t);
    void* GetMemory();
    const void* GetMemory() const;
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&, const ShaderImpl<NvnApi>*);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class TessellationStateImpl<NvnApi> : public DataContainer<TessellationStateImplData<NvnApi>> {
public:
    typedef TessellationStateInfo InfoType;

    TessellationStateImpl();
    ~TessellationStateImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class ViewportScissorStateImpl<NvnApi> {};

}  // namespace detail
}  // namespace nn::gfx