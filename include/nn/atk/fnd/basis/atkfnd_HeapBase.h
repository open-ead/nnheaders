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

    static const int DefaultAlignment = 4;

    static const u32 ExpHeapSignature   = 0x45585048; // HPXE
    static const u32 FrameHeapSignature = 0x46524D48; // HMRF
    static const u32 UnitHeapSignature  = 0x554E5448; // HTNU

    static const int OptionZeroClear  = 1 << 0;
    static const int OptionDebugFill  = 1 << 1;
    static const int OptionThreadSafe = 1 << 2;

    static const int ErrorPrint = 1;

    constexpr static int MIN_ALIGNMENT = DefaultAlignment;

    static HeapBase* FindContainHeap(const void* memBlock);
    static HeapBase* FindParentHeap(const HeapBase* pChild);
    
    void* GetHeapStartAddress();
    void* GetHeapEndAddress();

    size_t GetTotalSize();
    size_t GetTotalUsableSize();

    u32 SetFillValue(FillType type, u32 value);
    u32 GetFillValue(FillType type);

    HeapType GetHeapType();

protected:
    void Initialize(u32 signature, void* heapStart, void* heapEnd, u16 optFlag);
    void Finalize();
    
    u32 GetSignature() const;
    void* GetHeapStart() const;
    void* GetHeapEnd() const;
    
    void LockHeap();
    void UnlockHeap();

    void FillFreeMemory(void* address, size_t size);
    void FillNoUseMemory(void* address, size_t size);
    void FillAllocMemory(void* address, size_t size);

private:
    static HeapBase* FindContainHeap(HeapList* pList, const void* memBlock);
    static HeapList* FindListContainHeap(HeapBase* pHeapBase);

    u16 GetOptionFlag();
    void SetOptionFlag(u16 optFlag) { m_Attribute = optFlag & 0xff; };

    void* mHeapStart;
    void* mHeapEnd;
    u32 m_Signature;
    HeapList m_ChildList;
    u32 m_Attribute;
};
static_assert(sizeof(HeapBase) == 0x40);
} // namespace nn::atk::detail::fnd