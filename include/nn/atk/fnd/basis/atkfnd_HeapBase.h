#pragma once

#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::fnd {
class HeapBase : util::IntrusiveListBaseNode<HeapBase> {
public:
    using HeapList = util::IntrusiveList<HeapBase, util::IntrusiveListBaseNodeTraits<HeapBase>>;

    enum HeapType {
        HeapType_Exp,
        HeapType_Frame,
        HeapType_Unit,
        HeapTyep_Unknown,
    };

    enum FillType {
        FillType_NoUse,
        FillType_Alloc,
        FillType_Free,
        FillType_Max,
    };

    constexpr static u32 DefaultAlignment = 4;

    constexpr static s32 ExpHeapSignature = 0x45585048; // HPXE
    constexpr static s32 FrameHeapSignature = 0x46524D48; // HMRF
    constexpr static s32 UnitHeapSignature = 0x554E5448; // HTNU

    constexpr static u32 OptionZeroClear  = 1;
    constexpr static u32 OptionDebugFill  = 1 << 1;
    constexpr static u32 OptionThreadSafe = 1 << 2;

    constexpr static u32 ErrorPrint = 1;

    constexpr static u8 MIN_ALIGNMENT = 4;

    HeapList* FindListContainHeap();
    static HeapBase* FindContainHeap(HeapList* pList, const void* memBlock);
    static HeapBase* FindContainHeap(const void* memBlock);
    static HeapBase* FindParentHeap(HeapBase* pChild);

    u32 SetFillValue(FillType type, u32 value);
    u32 GetFillValue(FillType type);

    HeapType GetHeapType();

    void Initialize(u32 signature, void* heapStart, void* heapEnd, u16 optFlag);
    
    void SetOptionFlag(u16 optFlag);

    void FillNoUseMemory(void* address, size_t size);

    void Finalize();

    void LockHeap();
    void UnlockHeap();

    void FillFreeMemory(void* address, size_t size);

    u16 GetOptionFlag();

    void FillAllocMemory(void* address, size_t size);

private:
    void* mHeapStart;
    void* mHeapEnd;
    u32 m_Signature;
    HeapList m_ChildList;
    u32 m_Attribute;
};
static_assert(sizeof(HeapBase) == 0x40);
} // namespace nn::atk::detail::fnd