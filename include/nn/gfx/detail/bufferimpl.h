/**
 * @file bufferimpl.h
 * @brief Buffer implementation for GFX.
 */

#pragma once

#include <nn/types.h>
#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class GpuAddress;
class BufferInfo;

namespace detail {

template <>
class BufferImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;
    using MemoryPool = MemoryPoolImpl<NvnApi>;

    BufferImpl();
    ~BufferImpl();

    void Initialize(Device*, const BufferInfo&, MemoryPool*, s64, u64);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(s64, u64) const;
    void InvalidateMappedRange(s64, u64) const;
    void GetGpuAddress(GpuAddress*) const;

    void* mBuff;  // _0
};

template <>
class CommandBufferImpl<NvnApi> {
public:
    CommandBufferImpl();
    ~CommandBufferImpl();

    void Reset();
    void Begin();
    void End();
    void Dispatch(s32, s32, s32);

	u8 initialized; // 2 is set in sead::DrawContext (maybe some sort of staging enum?)
	u8 field_1;
	DeviceImpl<NvnApi>* device;
	char _10[8];
	NVNcommandBuffer field_18;
	NVNcommandBuffer* field_B8;
	NVNcommandHandle commandHandle;
	char _C8[32];
};

}  // namespace detail

}  // namespace nn::gfx
