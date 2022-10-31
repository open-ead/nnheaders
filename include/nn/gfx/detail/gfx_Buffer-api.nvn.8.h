#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_BufferData-api.nvn.8.h>
#include <nn/types.h>

namespace nn::gfx {

class BufferInfo;
class GpuAddress;

namespace detail {

template <>
class BufferImpl<NvnApi> : public DataContainer<BufferImplData<NvnApi>> {
public:
    using Device = DeviceImpl<NvnApi>;
    using MemoryPool = MemoryPoolImpl<NvnApi>;

    typedef BufferInfo InfoType;

    static const bool IsMemoryPoolRequired = true;

    static size_t GetBufferAlignment(Device*, const InfoType&);

    BufferImpl();
    ~BufferImpl();

    void Initialize(Device*, const BufferInfo&, MemoryPool*, ptrdiff_t, size_t);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    void GetGpuAddress(GpuAddress*) const;
};

}  // namespace detail

}  // namespace nn::gfx