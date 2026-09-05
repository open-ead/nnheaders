#include <nn/atk/atk_FrameHeap.h>

#include <cstring>

#include <nn/util/util_BytePtr.h>

#include <nn/atk/fnd/basis/atkfnd_Inlines.h>

namespace nn::atk::detail {

FrameHeap::FrameHeap() = default;

FrameHeap::Section::Section() = default;

FrameHeap::~FrameHeap() {
    Destroy();
}

bool FrameHeap::Create(void* startAddress, size_t size) {
    if (IsValid())
        Destroy();

    void* endAddress{util::BytePtr(startAddress, size).Get()};
    startAddress = util::BytePtr(startAddress).AlignUp(fnd::HeapBase::DefaultAlignment).Get();

    if (startAddress <= endAddress) {
        m_pHeap = fnd::FrameHeapImpl::Create(startAddress, fnd::GetOffsetFromPtr(startAddress, endAddress), 0);

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

void FrameHeap::Clear() {
    ClearSection();
    m_pHeap->Free(fnd::FrameHeapImpl::FreeAllMode);
    bool result = NewSection();
}

bool FrameHeap::NewSection() {
    void* buffer {m_pHeap->Alloc(sizeof(Section), fnd::HeapBase::DefaultAlignment)};

    if (buffer != nullptr) {
        std::memset(buffer, 0, sizeof(Section));
        auto* section{new (buffer) Section};
        m_SectionList.push_back(*section);

        return true;
    }

    return false;
}

}  // namespace nn::atk::detail