#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundMemoryAllocatable.h>

namespace nn::atk {
class SoundPlayer;

namespace detail {
class PlayerHeap;
class CallbackNode {
private:
    friend PlayerHeap;

    util::IntrusiveListNode m_Link;
    SoundMemoryAllocatable::DisposeCallback m_Callback;
    void* m_CallbackArg;
};
static_assert(sizeof(CallbackNode) == 0x20);

class PlayerHeap : SoundMemoryAllocatable {
public:
    using CallbackList = util::IntrusiveList<
                            CallbackNode, util::IntrusiveListMemberNodeTraits<
                                CallbackNode, &CallbackNode::m_Link>>;

    PlayerHeap();
    ~PlayerHeap() override;

    bool Create(void* startAddress, std::size_t size);
    void Clear();

    void* Allocate(std::size_t size) override;
    void* Allocate(std::size_t size, 
                   SoundMemoryAllocatable::DisposeCallback callback, void* callbackArg) override;

    std::size_t GetAllocateSize(std::size_t size, bool needMemoryPool) override;
    std::size_t GetFreeSize() const;

private:
    friend SoundPlayer;

    SoundPlayer* m_pPlayer;
    void* m_pStartAddress;
    void* m_pEndAddress;
    void* m_pAllocAddress;
    u8 m_State;
    u8 m_Padding[3];
    util::IntrusiveListNode m_Link;
    CallbackList m_CallbackList;
};
static_assert(sizeof(PlayerHeap) == 0x50);
} // namespace nn::atk::detail
} // namespace nn::atk