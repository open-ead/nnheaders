#include <nn/atk/atk_FrameHeap.h>

namespace nn::atk::detail {

FrameHeap::FrameHeap() = default;

bool FrameHeap::Create(void* startAddress, size_t size) {
    Destroy();

}

void FrameHeap::Destroy() {
    if (IsValid()) {
        ClearSection();
        m_pHeap->Free(fnd::FrameHeapImpl::FreeAllMode);
        m_pHeap->Destroy();
        m_pHeap = nullptr;
    }
}

}  // namespace nn::atk::detail