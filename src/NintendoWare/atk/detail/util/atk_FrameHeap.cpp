#include <nn/atk/atk_FrameHeap.h>

#include <cstring>

#include <nn/util/util_BytePtr.h>

#include <nn/atk/fnd/basis/atkfnd_Inlines.h>

namespace nn::atk::detail {

FrameHeap::FrameHeap() = default;

FrameHeap::~FrameHeap() {
    Destroy();
}

bool FrameHeap::Create(void* startAddress, size_t size) {
    if (IsValid())
        Destroy();

    void* endAddress{util::BytePtr(startAddress, size).Get()};
    startAddress = util::BytePtr(startAddress).AlignUp(fnd::HeapBase::DefaultAlignment).Get();

    if (startAddress <= endAddress) {
        m_pHeap = fnd::FrameHeapImpl::Create(startAddress,
                                             fnd::GetOffsetFromPtr(startAddress, endAddress), 0);

        if (m_pHeap != nullptr)
            return NewSection();
    }

    return false;
}

void FrameHeap::Destroy() {
    if (IsValid()) {
        ClearSection();
        m_pHeap->Free(fnd::FrameHeapImpl::FreeAllMode);
        m_pHeap->Destroy();
        m_pHeap = nullptr;
    }
}

void* FrameHeap::Alloc(size_t size, DisposeCallback callback, void* callbackArg,
                       HeapCallback heapCallback, void* heapCallbackArg) {
    const size_t blockSize{sizeof(Block)};
    const size_t allocSize{(size + 127) & ~63};

    void* mem{m_pHeap->Alloc(allocSize, blockSize)};

    if (mem == nullptr)
        return nullptr;

    void* buffer{util::BytePtr(mem, blockSize).Get()};

    Block* block{new (mem)
                     Block(buffer, size, callback, callbackArg, heapCallback, heapCallbackArg)};
    m_SectionList.back().AppendBlock(block);

    return buffer;
}

void FrameHeap::Clear() {
    ClearSection();
    m_pHeap->Free(fnd::FrameHeapImpl::FreeAllMode);
    [[maybe_unused]] bool result = NewSection();
}

// NON_MATCHING: incorrect branching
int FrameHeap::SaveState() {
    if (m_pHeap->RecordState(m_SectionList.size())) {
        if (NewSection()) {
            int result;
            
            result = m_SectionList.size() - 1;
            return result;
        }

        m_pHeap->FreeByState(0);
    }

    return -1;
}

bool FrameHeap::NewSection() {
    void* buffer{m_pHeap->Alloc(sizeof(Section), fnd::HeapBase::DefaultAlignment)};

    if (buffer != nullptr) {
        std::memset(buffer, 0, sizeof(Section));
        auto* section{new (buffer) Section};
        m_SectionList.push_back(*section);

        return true;
    }

    return false;
}

// NON_MATCHING: bad implementation
// Commented out to prevent inlining in other functions
// void FrameHeap::ClearSection() {
//     bool alreadyUseCallback{ProcessCallback(0)};

//     if (!m_SectionList.empty()) {
//         auto& section{m_SectionList.back()};

//         if (alreadyUseCallback)
//             section.SetUseCallback(false);

//         section.~Section();
//         for (auto it{m_SectionList.begin()}; it != m_SectionList.end(); ++it) {
//             m_SectionList.erase(it);

//         }
//     }
// }

FrameHeap::Section::Section() = default;

FrameHeap::Section::~Section() {
    m_BlockList.clear();
}

void FrameHeap::Section::AppendBlock(Block* block) {
    m_BlockList.push_back(*block);
}

}  // namespace nn::atk::detail