#pragma once

#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>

namespace nn::atk::detail::fnd {
class FrameHeapImpl : public HeapBase {
public:
    static const int FreeHeadMode = 1;
    static const int FreeTailMode = 2;
    static const int FreeAllMode  = 3;

    struct HeapState {
        u32 tagName;
        void* headAllocator;
        void* tailAllocator;
        HeapState* pPrevState;

        HeapState();
    };
    static_assert(sizeof(HeapState) == 0x20);

    FrameHeapImpl();

    static FrameHeapImpl* Create(void* startAddress, size_t size, u16 optFlag);

    void* Destroy();
    void* Alloc(size_t size, int alignment);
    size_t ResizeForMBlock(void* memBlock, size_t newSize);
    size_t GetAllocatableSize(int alignment);

    void Free(int mode);

    bool RecordState(u32 tagName);
    bool FreeByState(u32 tagName);

    u32 Adjust();

private:
    bool IsValid();

    void* AllocFromHead(size_t size, int alignment);
    void* AllocFromTail(size_t size, int alignment);

    void FreeHead();
    void FreeTail();

    void* m_pHeadAllocator;
    void* m_pTailAllocator;
    HeapState* m_pState;
};
static_assert(sizeof(FrameHeapImpl) == 0x58);
} // namespace nn::atk::detail::fnd