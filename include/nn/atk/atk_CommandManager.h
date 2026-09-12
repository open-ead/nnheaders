#pragma once

#include <atomic>
#include <nn/os/os_MessageQueueTypes.h>

#include <nn/atk/atk_Command.h>

namespace nn::atk::detail {

class CommandBuffer {
public:
    CommandBuffer();
    ~CommandBuffer();

    void Initialize(void* commandBuffer, size_t commandBufferSize);
    void Finalize();

    void* AllocMemory(size_t size);

    void FreeMemory(Command* lastCommand);

    size_t GetCommandBufferSize() const;
    size_t GetAllocatableCommandSize() const;
    size_t GetAllocatedCommandBufferSize() const;

private:
    u32* m_CommandMemoryArea;
    size_t m_CommandMemoryAreaSize;
    uintptr_t m_CommandMemoryAreaBegin;
    uintptr_t m_CommandMemoryAreaEnd;
    bool m_CommandMemoryAreaZeroFlag;
};
static_assert(sizeof(CommandBuffer) == 0x28);

class CommandManager {
public:
    using ProcessCommandListFunc = void (*)(Command*);
    using RequestProcessCommandFunc = void (*)();

    CommandManager();
    ~CommandManager();

    bool IsAvailable() const { return m_Available; }

    void Initialize(void* commandBuffer, size_t commandBufferSize, ProcessCommandListFunc func);
    void Finalize();

    void SetRequestProcessCommandFunc(RequestProcessCommandFunc func) {
        m_pRequestProcessCommandFunc = func;
    }

    template <typename CommandType>
    CommandType* AllocCommand() {
        return static_cast<CommandType*>(AllocMemory(sizeof(CommandType), true));
    }

    template <typename CommandType>
    CommandType* AllocCommand(bool forceProcessCommandFlag) {
        return static_cast<CommandType*>(AllocMemory(sizeof(CommandType), forceProcessCommandFlag));
    }

    u32 PushCommand(Command* command);

    u32 FlushCommand(bool forceFlag);
    u32 FlushCommand(bool forceFlag, bool forceProcessCommandFlag);

    void RecvCommandReply();
    void RecvCommandReplySync();

    void WaitCommandReply(u32 tag);

    bool IsFinishCommand(u32 tag) const;

    size_t GetCommandBufferSize() const;
    size_t GetAllocatableCommandSize() const;
    size_t GetAllocatedCommandBufferSize() const;

    int GetAllocatedCommandCount() const;

    u32 GetCommandListCount() const { return m_CommandListCount; }

    bool ProcessCommand();

private:
    void* AllocMemory(size_t size, bool forceProcessCommandFlag);
    bool TryAllocMemory(size_t size);

    void FinalizeCommandList(Command* command);

    static const int SendCommandQueueCount{32};
    static const int RecvCommandQueueCount{SendCommandQueueCount + 1};
    static const u32 InvalidCommand{0xffffffff};

    bool m_Available;
    ProcessCommandListFunc m_pProcessCommandListFunc;
    RequestProcessCommandFunc m_pRequestProcessCommandFunc;
    os::MessageQueueType m_SendCommandQueue;
    uintptr_t m_SendCommandQueueBuffer[SendCommandQueueCount];
    bool m_IsInitializedSendMessageQueue;
    os::MessageQueueType m_RecvCommandQueue;
    uintptr_t m_RecvCommandQueueBuffer[RecvCommandQueueCount];
    bool m_IsInitializedRecvMessageQueue;
    Command* m_CommandListBegin;
    Command* m_CommandListEnd;
    std::atomic_int m_CommandListCount;
    u32 m_CommandTag;
    u32 m_FinishCommandTag;
    CommandBuffer m_CommandBuffer;
    int m_AllocatedCommandCount;
};
static_assert(sizeof(CommandManager) == 0x310);

}  // namespace nn::atk::detail
