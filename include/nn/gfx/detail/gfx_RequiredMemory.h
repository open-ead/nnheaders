#pragma once

namespace nn::gfx::detail {

template <class TImpl>  // [with TImpl = BlendStateImpl<ApiVariation<ApiType<4>, ApiVersion<8>>>]
class RequiredMemory {
protected:
    static size_t GetRequiredMemorySize(...);
    static void SetMemory(void*, size_t);
    static void* GetMemory();
};

}  // namespace nn::gfx::detail