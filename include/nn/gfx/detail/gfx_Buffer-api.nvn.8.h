#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_BufferData-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/types.h>

namespace nn::gfx {

class BufferInfo;
class BufferTextureViewInfo;
class GpuAddress;

namespace detail {

template <>
class BufferImpl<NvnApi> : public DataContainer<BufferImplData<NvnApi>> {
    NN_NO_COPY(BufferImpl);

public:
    typedef BufferInfo InfoType;

    static const bool IsMemoryPoolRequired = true;

    static size_t GetBufferAlignment(DeviceImpl<NvnApi>*, const InfoType&);

    BufferImpl();
    ~BufferImpl();

    void Initialize(DeviceImpl<NvnApi>*, const BufferInfo&, MemoryPoolImpl<NvnApi>*, ptrdiff_t,
                    size_t);
    void Finalize(DeviceImpl<NvnApi>*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    void GetGpuAddress(GpuAddress*) const;
};

template <>
class BufferTextureViewImpl<NvnApi> : public DataContainer<BufferTextureViewImplData<NvnApi>> {
    NN_NO_COPY(BufferTextureViewImpl);

public:
    typedef BufferTextureViewInfo InfoType;

    static size_t GetOffsetAlignment(DeviceImpl<NvnApi>*, const InfoType&);

    BufferTextureViewImpl();
    ~BufferTextureViewImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

}  // namespace detail

}  // namespace nn::gfx