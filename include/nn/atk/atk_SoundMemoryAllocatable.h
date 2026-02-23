#pragma once

#include <nn/types.h>

namespace nn::atk {
class SoundMemoryAllocatable {
public:
    using DisposeCallback = void(*)(void*);

    virtual ~SoundMemoryAllocatable() = 0;
    virtual void* Allocate(size_t size) = 0;
    virtual void* Allocate(size_t size, DisposeCallback callback, void* callbackArg) = 0;
    virtual std::size_t GetAllocateSize(size_t size, bool needMemoryPool) = 0;
};
} // namespace nn::atk