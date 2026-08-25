#include <cstring>

#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>
#include <nn/atk/fnd/basis/atkfnd_Inlines.h>

namespace nn::atk::detail::fnd {
namespace {
HeapBase::HeapList sRootList;
u32 sFillVals[HeapBase::FillType_Max] = { 0xC3C3C3C3, 0xF3F3F3F3, 0xD3D3D3D3 };
} // anonymous namespace

HeapBase::HeapList* HeapBase::FindListContainHeap(HeapBase* pHeapBase) {
    HeapList* pList {&sRootList};
    HeapBase* pContainHeapBase {FindContainHeap(pList, pHeapBase)};

    if (pContainHeapBase != nullptr)
        pList = &pContainHeapBase->m_ChildList;

    return pList;
}

// NON_MATCHING: commented because it needs to not be inlined for other functions to match
// HeapBase* HeapBase::FindContainHeap(HeapList* pList, const void* memBlock) {
//     uintptr_t memBlockAddress {reinterpret_cast<uintptr_t>(memBlock)};

//     for (auto itr {pList->begin()}; pList->end() != itr; ++itr) {
//         auto curItr {++itr};

//         if (&pList->front() == &*curItr)
//             return nullptr;

//         if (reinterpret_cast<u64>(curItr->mHeapStart) <= memBlockAddress && memBlockAddress < reinterpret_cast<u64>(curItr->mHeapEnd)) {
//             HeapBase* pChildHeapBase {FindContainHeap(&curItr->m_ChildList, &*curItr)};

//             if (pChildHeapBase != nullptr)
//                 return pChildHeapBase;

//         }
//     }

//     return nullptr;
// }

HeapBase* HeapBase::FindContainHeap(const void* memBlock) {
    return FindContainHeap(&sRootList, memBlock);
}

HeapBase* HeapBase::FindParentHeap(const HeapBase* pChild) {
    u64 heapAddress {reinterpret_cast<u64>(pChild)};

    for (auto itr {sRootList.begin()}; itr != sRootList.end();) {
        auto curItr {itr++};

        if (reinterpret_cast<u64>(curItr->mHeapStart) <= heapAddress && reinterpret_cast<u64>(curItr->mHeapEnd) > heapAddress)
            return FindContainHeap(&curItr->m_ChildList, &*curItr);
    }

    return nullptr;
}

u32 HeapBase::SetFillValue(FillType type, u32 val) {
    u32 oldVal {sFillVals[type]};
    sFillVals[type] = val;

    return oldVal;
}

u32 HeapBase::GetFillValue(FillType type) {
    return sFillVals[type];
}

HeapBase::HeapType HeapBase::GetHeapType() {
    switch (m_Signature) {
    case ExpHeapSignature:
        return HeapType::HeapType_Exp;

    case FrameHeapSignature:
        return HeapType::HeapType_Frame;

    case UnitHeapSignature:
        return HeapType::HeapType_Unit;

    default:
        return HeapType::HeapType_Unknown;
    }
}

void HeapBase::Initialize(u32 signature, void* heapStart, void* heapEnd, u16 optFlag) {
    m_Signature = signature;
    mHeapStart = heapStart;
    mHeapEnd = heapEnd;

    SetOptionFlag(optFlag);
    u32 diff = GetOffsetFromPtr(heapStart, heapEnd);
    FillNoUseMemory(heapStart, diff);

    HeapList* pList {FindListContainHeap(this)};

    pList->push_back(*this);
}

// NON_MATCHING
void HeapBase::Finalize() {
    HeapList* pList {FindListContainHeap(this)};
    
    if (pList != nullptr)
        pList->erase(pList->begin());

    m_Signature = 0;
}

void HeapBase::LockHeap() {}

void HeapBase::UnlockHeap() {}

void HeapBase::FillFreeMemory(void* address, size_t size) {
    if ((m_Attribute & OptionDebugFill) != 0)
        memset(address, static_cast<int>(GetFillValue(FillType_Free)), size);
}

void HeapBase::FillNoUseMemory(void* address, size_t size) {
    if ((m_Attribute & OptionDebugFill) != 0)
        memset(address, static_cast<int>(GetFillValue(FillType_NoUse)), size);
}

void HeapBase::FillAllocMemory(void* address, size_t size) {
    if (m_Attribute & OptionZeroClear) {
        memset(address, 0, size);
        return;
    }

    if ((m_Attribute & OptionDebugFill) != 0)
        memset(address, static_cast<int>(GetFillValue(FillType_Alloc)), size);
}


u16 HeapBase::GetOptionFlag() {
    return m_Attribute & 0xFF;
}

// NON_MATCHING: matches inlined, but not standalone
void HeapBase::SetOptionFlag(u16 optFlag) {
    if (optFlag >= 0) {
        u32 maskBits {0xff};
        u32 newVal = optFlag & maskBits;
        m_Attribute = newVal;
    }
}
} // namespace nn::atk::detail::fnd