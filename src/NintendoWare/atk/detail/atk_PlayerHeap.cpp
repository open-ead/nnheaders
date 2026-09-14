#include <nn/atk/atk_PlayerHeap.h>

#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>

namespace nn::atk::detail {

PlayerHeap::PlayerHeap() = default;

PlayerHeap::~PlayerHeap() {
    Destroy();
    m_State = 0;
}

void PlayerHeap::Destroy() {
    Clear();
    m_pAllocAddress = nullptr;
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

}  // namespace nn::atk::detail
