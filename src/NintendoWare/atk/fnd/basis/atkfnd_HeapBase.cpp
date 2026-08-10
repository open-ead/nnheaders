#include <cstring>
#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>
#include "nn/atk/fnd/basis/atkfnd_Inlines.h"

namespace nn::atk::detail::fnd {
static HeapBase::HeapList sRootList;
u32 sFillVals[HeapBase::FillType_Max] { 0xC3C3C3C3, 0xF3F3F3F3, 0xD3D3D3D3 };

// NON-MATCHING: unknown reason. Possibly something to do with global variables
void HeapBase::Initialize(u32 signature, void* heapStart, void* heapEnd, u16 optFlag) {
    m_Signature = signature;
    mHeapStart = heapStart;
    mHeapEnd = heapEnd;

    SetOptionFlag(optFlag);
    u32 diff  = GetOffsetFromPtr(heapStart, heapEnd);
    FillNoUseMemory(heapStart, diff);

    HeapList* pList {FindListContainHeap(this)};

    pList->push_back(*this);
}

// NON-MATCHING
void HeapBase::FillFreeMemory(void* address, size_t size) {
    if ((m_Attribute >> 1 & 1) != 0)
        memset(address, static_cast<int>(GetFillValue(FillType_Free)), size);
}

// NON-MATCHING
void HeapBase::FillAllocMemory(void* address, size_t size) {
    if (m_Attribute & 1) {
        memset(address, 0, size);
        return;
    }

    if ((m_Attribute >> 1 & 1) != 0)
        memset(address, GetFillValue(FillType_Alloc), size);
}

// NON-MATCHING
void HeapBase::FillNoUseMemory(void* address, size_t size) {
    if ((m_Attribute >> 1 & 1) != 0)
        memset(address, (int)GetFillValue(FillType_NoUse), size);
}

// Inlined
u32 HeapBase::GetFillValue(FillType type) {
    return sFillVals[type];
}

// Inlined
HeapBase::HeapList* HeapBase::FindListContainHeap(HeapBase* pHeapBase) {
    HeapList* pList {&sRootList};
    HeapBase* pContainHeapBase {FindContainHeap(pList, pHeapBase)};

    if (pContainHeapBase != nullptr)
        pList = &pContainHeapBase->m_ChildList;

    return pList;
}
} // namespace nn::atk::detail::fnd