#include <nn/atk/atk_PlayerHeap.h>

#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>

namespace nn::atk::detail {

PlayerHeap::PlayerHeap() = default;

PlayerHeap::~PlayerHeap() {
    Destroy();
    m_State = 0;
}

bool PlayerHeap::Create(void* startAddress, size_t size) {
    Util::IsValidMemoryForDsp(startAddress, size);

    void* endAddress{util::BytePtr(startAddress, static_cast<ptrdiff_t>(size)).Get()};
    startAddress = util::BytePtr(startAddress).AlignUp(fnd::Thread::StackAlignment).Get();

    if (startAddress <= endAddress) {
        m_pStartAddress = startAddress;
        m_pEndAddress = endAddress;
        m_pAllocAddress = startAddress;
        return true;
    }

    return false;
}

void PlayerHeap::Destroy() {
    Clear();
    m_pAllocAddress = nullptr;
}

void* PlayerHeap::Allocate(size_t size) {
    void* endp{util::BytePtr(m_pAllocAddress, static_cast<ptrdiff_t>(size)).Get()};

    if (endp > m_pEndAddress)
        return nullptr;

    void* allocAddress{m_pAllocAddress};
    m_pAllocAddress = util::BytePtr(endp).AlignUp(fnd::Thread::StackAlignment).Get();

    return allocAddress;
}

void* PlayerHeap::Allocate(size_t size, DisposeCallback callback, void* callbackArg) {
    void* callbackNodeBuffer{util::BytePtr(m_pAllocAddress, static_cast<ptrdiff_t>(size)).Get()};
    void* endp{util::BytePtr(callbackNodeBuffer, sizeof(CallbackNode)).Get()};

    if (endp > m_pEndAddress)
        return nullptr;

    void* allocAddress{m_pAllocAddress};
    m_pAllocAddress = util::BytePtr(endp).AlignUp(fnd::Thread::StackAlignment).Get();

    CallbackNode* node{new (callbackNodeBuffer) CallbackNode};
    node->SetCallback(callback);
    node->SetCallbackArg(callbackArg);
    m_CallbackList.push_back(*node);

    return allocAddress;
}

size_t PlayerHeap::GetAllocateSize(size_t size, [[maybe_unused]] bool needMemoryPool) {
    return size;
}

}  // namespace nn::atk::detail
