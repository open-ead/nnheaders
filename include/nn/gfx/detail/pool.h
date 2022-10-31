#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/types.h>
#include <nvn/nvn_types.h>

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
class MemoryPoolImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;

    MemoryPoolImpl();
    ~MemoryPoolImpl();

    void Initialize(Device*, MemoryPoolInfo const&);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(s64, u64) const;
    void InvalidateMappedRange(s64, u64) const;

    NVNmemoryPool* pnPool;
    void* field_8;
    NVNmemoryPool nPool;
    u8 _110[0x10];  // pool data
};

}  // namespace detail

}  // namespace nn::gfx
