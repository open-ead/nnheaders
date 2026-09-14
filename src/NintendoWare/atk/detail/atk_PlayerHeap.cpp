#include <nn/atk/atk_PlayerHeap.h>

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

}  // namespace nn::atk::detail
