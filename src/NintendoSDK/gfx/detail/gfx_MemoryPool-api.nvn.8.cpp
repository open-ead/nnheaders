#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

size_t MemoryPoolImpl<NvnApi>::GetPoolMemoryAlignment(DeviceImpl<NvnApi>*, const MemoryPoolInfo&) {
    return 0x1000;
}

size_t MemoryPoolImpl<NvnApi>::GetPoolMemorySizeGranularity(DeviceImpl<NvnApi>*,
                                                            const MemoryPoolInfo&) {
    return 0x1000;
}

MemoryPoolImpl<NvnApi>::MemoryPoolImpl() {
    state = State_NotInitialized;
}

MemoryPoolImpl<NvnApi>::~MemoryPoolImpl() {}

void MemoryPoolImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* pDevice, const MemoryPoolInfo& info) {
    int memoryPoolFlags = Nvn::GetMemoryPoolFlags(info.GetMemoryPoolProperty());

    pNvnMemoryPool = &nvnMemoryPool;
    pMemory = info.GetPoolMemory();

    NVNmemoryPoolBuilder builder;
    nvnMemoryPoolBuilderSetDevice(&builder, pDevice->ToData()->pNvnDevice);
    nvnMemoryPoolBuilderSetDefaults(&builder);
    nvnMemoryPoolBuilderSetFlags(&builder, memoryPoolFlags);
    nvnMemoryPoolBuilderSetStorage(&builder, info.GetPoolMemory(), info.GetPoolMemorySize());
    NVNboolean isMemoryPoolOk = nvnMemoryPoolInitialize(pNvnMemoryPool, &builder);

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void MemoryPoolImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    nvnMemoryPoolFinalize(pNvnMemoryPool);
    pMemory = nullptr;
    state = State_NotInitialized;
}

void* MemoryPoolImpl<NvnApi>::Map() const {
    return nvnMemoryPoolMap(pNvnMemoryPool);
}

void MemoryPoolImpl<NvnApi>::Unmap() const {}

void MemoryPoolImpl<NvnApi>::FlushMappedRange(ptrdiff_t offset, size_t size) const {
    nvnMemoryPoolFlushMappedRange(pNvnMemoryPool, offset, size);
}

void MemoryPoolImpl<NvnApi>::InvalidateMappedRange(ptrdiff_t offset, size_t size) const {
    nvnMemoryPoolInvalidateMappedRange(pNvnMemoryPool, offset, size);
}

}  // namespace nn::gfx::detail