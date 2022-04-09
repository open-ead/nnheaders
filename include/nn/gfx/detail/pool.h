#pragma once

#include <nn/types.h>

namespace nn::gfx {

struct MemoryPoolInfo;

namespace detail {

template <typename T>
struct DeviceImpl;

class MemoryPoolData {
public:
    void SetDefault();

    s32 _0;  // set to 0x88
    s32 _4;
    u64 _8;
};

template <typename Api>
class MemoryPoolImpl {
public:
    using Device = DeviceImpl<Api>;

    MemoryPoolImpl();
    ~MemoryPoolImpl();

    void Initialize(Device*, MemoryPoolInfo const&);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(s64, u64) const;
    void InvalidateMappedRange(s64, u64) const;

    u8 _0[0x120];  // pool data
};

}  // namespace detail

}  // namespace nn::gfx
