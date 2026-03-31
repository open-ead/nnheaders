#pragma once

#include "nn/gfx/gfx_Device.h"

namespace nn::g3d {
class MaterialObj {
public:
    struct InitializeArgument {
        s32 CalculateMemorySize();
    };

    void Initialize(const InitializeArgument&, void*, u64);
    void InitializeDependPointer();
    void GetBlockBufferAlignment(gfx::TDevice<gfx::ApiVariationNvn8>*) const;
    void CalculateBlockBufferSize(gfx::TDevice<gfx::ApiVariationNvn8>*) const;
    void SetupBlockBufferImpl(gfx::TDevice<gfx::ApiVariationNvn8>*,
                              gfx::TMemoryPool<gfx::ApiVariationNvn8>*, s64, u64);
    void ResetDirtyFlags();
    void SetupBlockBuffer(gfx::TDevice<gfx::ApiVariationNvn8>*,
                          gfx::TMemoryPool<gfx::ApiVariationNvn8>*, s64, u64);
    void CleanupBlockBuffer(gfx::TDevice<gfx::ApiVariationNvn8>*);
    void CalculateMaterial(s32);

    template <bool>
    void ConvertDirtyParams(void*, u32*);

private:
    void* filler[16];
};
}  // namespace nn::g3d
