#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundMemoryAllocatable.h>

namespace nn::atk {

class SoundPlayer;

namespace detail {

class PlayerHeap;
class CallbackNode {
public:
    void SetCallback(SoundMemoryAllocatable::DisposeCallback callback) { m_Callback = callback; }
    void SetCallbackArg(void* callbackArg) { m_CallbackArg = callbackArg; }

    SoundMemoryAllocatable::DisposeCallback GetCallback() { return m_Callback; }
    void* GetCallbackArg() { return m_CallbackArg; }

    util::IntrusiveListNode m_Link;

private:
    SoundMemoryAllocatable::DisposeCallback m_Callback;
    void* m_CallbackArg;
};
static_assert(sizeof(CallbackNode) == 0x20);

class PlayerHeap : public SoundMemoryAllocatable {
public:
    PlayerHeap();
    ~PlayerHeap() override;

    void AttachSoundPlayer(SoundPlayer* player) { m_pPlayer = player; }

    bool Create(void* startAddress, size_t size);
    void Destroy();

    void* Allocate(size_t size) override;
    void* Allocate(size_t size, DisposeCallback callback, void* callbackArg) override;

    size_t GetAllocateSize(size_t size, bool needMemoryPool) override;

    void Clear();

    bool IsValid() const { return m_pPlayer != nullptr; }

    size_t GetFreeSize() const;

    enum State { State_Constructed, State_TaskRegistered, State_TaskFinished, State_Destructed };

    State GetState() const { return static_cast<State>(m_State); }
    void SetState(State state) { m_State = static_cast<u8>(state); }

private:
    using CallbackList = util::IntrusiveList<
        CallbackNode, util::IntrusiveListMemberNodeTraits<CallbackNode, &CallbackNode::m_Link>>;

    SoundPlayer* m_pPlayer{};
    void* m_pStartAddress{};
    void* m_pEndAddress{};
    void* m_pAllocAddress{};
    u8 m_State{State_Constructed};
    u8 m_Padding[3];

public:
    util::IntrusiveListNode m_Link;
    CallbackList m_CallbackList;
};
static_assert(sizeof(PlayerHeap) == 0x50);

}  // namespace detail
}  // namespace nn::atk
