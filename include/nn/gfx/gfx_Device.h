#pragma once

#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>
#include <nn/util.h>

namespace nn::gfx {
class DeviceInfo;

template <typename TTarget>
class TCommandBuffer;

template <typename TType, typename TVersion>
class ApiVariation;

template <int T>
class ApiType;

template <int T>
class ApiVersion;

template <class TTarget>
class TDevice : public detail::DeviceImpl<TTarget>,
                private detail::RequiredMemory<detail::DeviceImpl<TTarget>> {
    NN_NO_COPY(TDevice);

public:
    typedef DeviceInfo InfoType;

    static size_t GetRequiredMemorySize(const InfoType&);

    TDevice();
    void Initialize(const InfoType&);
    void Finalize();
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

typedef TDevice<ApiVariation<ApiType<4>, ApiVersion<8>>> Device;
typedef TCommandBuffer<gfx::ApiVariation<gfx::ApiType<4>, gfx::ApiVersion<8>>> CommandBuffer;

}  // namespace nn::gfx
