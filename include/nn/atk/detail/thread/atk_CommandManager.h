#pragma once

#include <atomic>
#include <nn/os/os_MessageQueueTypes.h>

#include <nn/atk/detail/thread/atk_Command.h>

namespace nn::atk::detail {
class CommandBuffer {
public:
    CommandBuffer();
    ~CommandBuffer();

    void Finalize();

    void Initialize(void* buffer, size_t bufferSize);

    void* AllocMemory(size_t size);

    void FreeMemory(Command* command);

    size_t GetCommandBufferSize() const;
    size_t GetAllocatableCommandSize() const;
    size_t GetAllocatedCommandBufferSize() const;

private:
    u32* m_CommandMemoryArea;
    size_t m_CommandMemoryAreaSize;
    std::uintptr_t m_CommandMemoryAreaBegin;
    std::uintptr_t m_CommandMemoryAreaEnd;
    bool m_CommandMemoryAreaZeroFlag;
};
static_assert(sizeof(CommandBuffer) == 0x28);

class CommandManager {
public:
    using ProcessCommandListFunc = void(*)(Command*);
    using RequestProcessCommandFunc = void(*)();

    constexpr static s32 SendCommandQueueCount = 32;
    constexpr static s32 RecvCommandQueueCount = 33;
    constexpr static s32 InvalidCommand = -1;

    CommandManager();
    ~CommandManager();

    void Finalize();
    void Initialize(void* commandBuffer, size_t commandBufferSize, ProcessCommandListFunc func);

    void* AllocMemory(size_t size, bool forceProcessCommandFlag);

    bool TryAllocMemory(size_t size);

    void RecvCommandReply();

    u32 FlushCommand(bool forceFlag, bool forceProcessCommandFlag);

    void WaitCommandReply(u32 tag);

    void RecvCommandReplySync();

    u32 PushCommand(Command* command);
    u32 FlushCommand(bool forceFlag);

    void FinalizeCommandList(Command* command);

    bool IsFinishCommand(u32) const;

    size_t GetCommandBufferSize() const;
    size_t GetAllocatableCommandSize() const;
    size_t GetAllocatedCommandBufferSize() const;

    s32 GetAllocatedCommandCount() const;

    bool ProcessCommand();

private:
    bool m_Available;
    ProcessCommandListFunc m_pProcessCommandListFunc;
    RequestProcessCommandFunc m_pRequestProcessCommandFunc;
    os::MessageQueueType m_SendCommandQueue;
    std::uintptr_t m_SendCommandQueueBuffer[SendCommandQueueCount];
    bool m_IsInitializedSendMessageQueue;
    os::MessageQueueType m_RecvCommandQueue;
    std::uintptr_t m_RecvCommandQueueBuffer[RecvCommandQueueCount];
    bool m_IsInitializedRecvMessageQueue;
    Command* m_CommandListBegin;
    Command* m_CommandListEnd;
    std::atomic_int m_CommandListCount;
    u32 m_CommandTag;
    u32 m_FinishCommandTag;
    CommandBuffer m_CommandBuffer;
    s32 m_AllocatedCommandCount;
};
static_assert(sizeof(CommandManager) == 0x310);
} // namespace nn::atk::detail