/**
 * @file bufferimpl.h
 * @brief Buffer implementation for GFX.
 */

#pragma once

#include <nn/types.h>

namespace nn::gfx {

class GpuAddress;
class BufferInfo;

namespace detail {

template <typename Api>
class MemoryPoolImpl;
template <typename Api>
class DeviceImpl;

template <typename Api>
class BufferImpl {
public:
    using Device = DeviceImpl<Api>;
    using MemoryPool = MemoryPoolImpl<Api>;

    BufferImpl();
    ~BufferImpl();

    void Initialize(Device*, BufferInfo const&, MemoryPool*, s64, u64);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(s64, u64) const;
    void InvalidateMappedRange(s64, u64) const;
    void GetGpuAddress(GpuAddress*) const;

    void* mBuff;  // _0
};

template <typename T>
class CommandBufferImpl {
public:
    CommandBufferImpl();
    ~CommandBufferImpl();

    void Reset();
    void Begin();
    void End();
    void Dispatch(s32, s32, s32);
};

}  // namespace detail

}  // namespace nn::gfx
