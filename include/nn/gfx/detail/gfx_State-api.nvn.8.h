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
    using Device = DeviceImpl<NvnApi>;
    typedef RasterizerStateInfo InfoType;

    RasterizerStateImpl();
    ~RasterizerStateImpl();

    void Initialize(Device* device, const InfoType& info);
    void Finalize(Device*);
};

template <>
class BlendStateImpl<NvnApi> : public DataContainer<BlendStateImplData<NvnApi>> {
public:
    using Device = DeviceImpl<NvnApi>;
    typedef BlendStateInfo InfoType;

    static size_t GetRequiredMemorySize(const InfoType& info);

    BlendStateImpl();
    ~BlendStateImpl();

    void SetMemory(void* p, size_t s);
    void* GetMemory();
    void* GetMemory() const;

    void Initialize(Device* device, const InfoType& info);
    void Finalize(Device* device);
};

template <>
class DepthStencilStateImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;

    DepthStencilStateImpl();
    ~DepthStencilStateImpl();

    void Initialize(Device*, const DepthStencilStateInfo& info);
    void Finalize(Device*);
};

template <>
class VertexStateImpl<NvnApi> {};

template <>
class TessellationStateImpl<NvnApi> {};

}  // namespace detail
}  // namespace nn::gfx