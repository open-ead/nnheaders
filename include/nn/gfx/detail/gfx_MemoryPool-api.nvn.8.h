#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_MemoryPoolData-api.nvn.8.h>

namespace nn::gfx {

struct MemoryPoolInfo;

namespace detail {

class MemoryPoolData {
public:
    void SetDefault();

    s32 _0;  // set to 0x88
    s32 _4;
    u64 _8;
};

template <>
class MemoryPoolImpl<NvnApi> : public DataContainer<MemoryPoolImplData<NvnApi>> {
    NN_NO_COPY(MemoryPoolImpl);

public:
    typedef MemoryPoolInfo InfoType;

    static size_t GetPoolMemoryAlignment(DeviceImpl<NvnApi>*, const InfoType&);
    static size_t GetPoolMemorySizeGranularity(DeviceImpl<NvnApi>*, const InfoType&);

    MemoryPoolImpl();
    ~MemoryPoolImpl();

    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
};

}  // namespace detail

}  // namespace nn::gfx