/**
 * @file mem.h
 * @brief Memory allocation functions.
 */

#pragma once

#include <nn/os.h>
#include <nn/types.h>
#include <nn/util/util_TypedStorage.h>

namespace nn {
namespace nlibsdk {
namespace heap {
class CentralHeap;
}  // namespace heap
}  // namespace nlibsdk

namespace mem {
class StandardAllocator {
public:
    StandardAllocator();
    StandardAllocator(void*, u64);
    StandardAllocator(void*, u64, bool);

    ~StandardAllocator() {
        if (mIsInitialized) {
            Finalize();
        }
    }

    void Initialize(void* address, u64 size);
    void Initialize(void* address, u64 size, bool enableCache);
    void Finalize();
    void* Reallocate(void* address, u64 newSize);
    void* Allocate(u64 size);
    void* Allocate(u64 size, u64 alignment);
    void Free(void* address);

    u64 GetSizeOf(const void* address) const;
    void ClearThreadCache() const;
    void CleanUpManagementArea() const;
    u64 GetTotalFreeSize() const;
    u64 GetAllocatableSize() const;
    void Dump() const;

    bool mIsInitialized;
    bool mIsEnabledThreadCache;
    uintptr_t mAllocAddr;
    nn::os::TlsSlot mTlsSlot;
    nn::util::TypedStorage<nn::nlibsdk::heap::CentralHeap, 48, 8> mCentralHeapStorage;
};

static_assert(sizeof(StandardAllocator) == 0x48);

}  // namespace mem
}  // namespace nn