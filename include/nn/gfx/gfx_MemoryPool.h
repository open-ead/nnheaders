#pragma once

#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Device.h>

namespace nn::gfx {

template <class TTarget>
class TMemoryPool : public detail::MemoryPoolImpl<TTarget> {
    NN_NO_COPY(TMemoryPool);

public:
    typedef MemoryPoolInfo InfoType;

    using detail::MemoryPoolImpl<TTarget>::MemoryPoolImpl;

    static size_t GetPoolMemoryAlignment(TDevice<TTarget>*, const InfoType&);
    static size_t GetPoolMemorySizeGranularity(TDevice<TTarget>*, const InfoType&);

    void Initialize(TDevice<TTarget>* device, const InfoType& info, const char* debugLabel) {
        new (this) TMemoryPool<TTarget>();
        detail::MemoryPoolImpl<TTarget>::Initialize(device, info);
        util::SetMemoryPoolDebugLabel(this, debugLabel);
    }

    void Finalize(TDevice<TTarget>* device) {
        detail::MemoryPoolImpl<TTarget>::Finalize(device);
        this->~TMemoryPool<TTarget>();
    }

    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx
