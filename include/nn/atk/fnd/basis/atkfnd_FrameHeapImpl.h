#pragma once

#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>

namespace nn::atk::detail::fnd {
class FrameHeapImpl : HeapBase {
public:
    constexpr static s32 FreeHeadMode = 1;
    constexpr static s32 FreeTailMode = 2;
    constexpr static s32 FreeAllMode = 3;

    struct HeapState {
        u32 tagName;
        void* headAllocator;
        void* tailAllocator;
        HeapState* pPrevState; 
    };
    static_assert(sizeof(HeapState) == 0x20);

    static FrameHeapImpl* Create(void* startAddress, size_t size, u16 optFlag);
    
    void* Destroy();

    void* Alloc(size_t size, s32 alignment);
    void* AllocFromHead(size_t size, s32 alignment);
    void* AllocFromTail(size_t size, s32 alignment);

    u64 ResizeForMBlock(void* memBlock, size_t size);

    size_t GetAllocatableSize(s32);

    void Free(s32 mode);
    void FreeHead();
    void FreeTail();

    bool RecordState(u32);

    bool FreeByState(u32);

    s32 Adjust();

private:
    void* m_pHeadAllocator;
    void* m_pTailAllocator;
    HeapState* m_pState;
};
static_assert(sizeof(FrameHeapImpl) == 0x58);
} // namespace nn::atk::detail::fnd