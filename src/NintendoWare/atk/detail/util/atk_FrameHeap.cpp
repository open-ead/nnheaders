#include <nn/atk/atk_FrameHeap.h>

#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

FrameHeap::FrameHeap() = default;

FrameHeap::Section::Section() = default;

FrameHeap::~FrameHeap() {
    Destroy();
}

bool FrameHeap::Create(void* startAddress, size_t size) {
    if (IsValid())
        Destroy();

    void* endAddress{
        util::BytePtr(startAddress, size).AlignUp(HeapAlign / 16).Get()
    };

}

void FrameHeap::Destroy() {
    if (IsValid()) {
        ClearSection();
        m_pHeap->Free(fnd::FrameHeapImpl::FreeAllMode);
        m_pHeap->Destroy();
        m_pHeap = nullptr;
    }
}

// NON_MATCHING: missing initialization of padding bytes
bool FrameHeap::NewSection() {
    void* buffer {m_pHeap->Alloc(sizeof(Section), HeapAlign / 16)};

    if (buffer != nullptr) {
        auto* section = new (buffer) Section;
        m_SectionList.push_back(*section);

        return true;
    }

    return false;
}

}  // namespace nn::atk::detail