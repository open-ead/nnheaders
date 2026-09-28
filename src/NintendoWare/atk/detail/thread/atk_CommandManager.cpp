#include <nn/atk/atk_CommandManager.h>

namespace nn::atk::detail {

CommandBuffer::CommandBuffer() = default;

CommandBuffer::~CommandBuffer() = default;

void CommandBuffer::Finalize() {}

void CommandBuffer::Initialize(void* commandBuffer, size_t commandBufferSize) {
    m_CommandMemoryAreaBegin = 0;
    m_CommandMemoryAreaEnd = 0;
    m_CommandMemoryAreaZeroFlag = false;

    m_CommandMemoryArea = static_cast<u32*>(commandBuffer);
    m_CommandMemoryAreaSize = commandBufferSize / sizeof(u32);
}

// NON_MATCHING
void* CommandBuffer::AllocMemory(size_t size) {
    if (m_CommandMemoryAreaZeroFlag)
        return nullptr;

    void* ptr{};

    const size_t count{(size + 3) / 4};

    volatile uintptr_t curEnd{m_CommandMemoryAreaEnd};

    if (count + m_CommandMemoryAreaBegin > curEnd)
        return nullptr;

    if (m_CommandMemoryAreaBegin < curEnd) {
        ptr = count + m_CommandMemoryArea;
        m_CommandMemoryAreaBegin = count + m_CommandMemoryAreaBegin;
    } else if (m_CommandMemoryAreaSize < m_CommandMemoryAreaBegin + count) {
        if (count > curEnd)
            return nullptr;

        ptr = m_CommandMemoryArea;
        m_CommandMemoryAreaBegin = count;
    }

    Command* command{static_cast<Command*>(ptr)};
    if (command != nullptr) {
        if (count == curEnd) {
            m_CommandMemoryAreaZeroFlag = true;
        }
        command->memory_next = count;
    }

    return command;
}

void CommandBuffer::FreeMemory(Command* lastCommand) {
    m_CommandMemoryAreaEnd = lastCommand->memory_next;
    m_CommandMemoryAreaZeroFlag = false;
}

size_t CommandBuffer::GetCommandBufferSize() const {
    return m_CommandMemoryAreaSize * sizeof(u32);
}

// NON_MATCHING
size_t CommandBuffer::GetAllocatableCommandSize() const {
    if (m_CommandMemoryAreaZeroFlag)
        return 0;

    size_t count{m_CommandMemoryAreaSize};

    volatile uintptr_t curBegin{m_CommandMemoryAreaBegin};
    volatile uintptr_t curEnd{m_CommandMemoryAreaEnd};

    if (curBegin < curEnd)
        return (curEnd - curBegin) * 4;

    if (curEnd > count - curBegin)
        return 0;

    return (count - curBegin) * 4;
}

CommandManager::CommandManager() = default;

CommandManager::~CommandManager() {
    Finalize();
}

void CommandManager::Finalize() {
    if (m_IsInitializedSendMessageQueue) {
        os::FinalizeMessageQueue(&m_SendCommandQueue);
        m_IsInitializedSendMessageQueue = false;
    }

    if (m_IsInitializedRecvMessageQueue) {
        os::FinalizeMessageQueue(&m_RecvCommandQueue);
        m_IsInitializedRecvMessageQueue = false;
    }

    m_Available = false;
}

void CommandManager::Initialize(void* commandBuffer, size_t commandBufferSize,
                                ProcessCommandListFunc func) {
    m_pProcessCommandListFunc = func;
    m_pRequestProcessCommandFunc = nullptr;
    m_CommandBuffer.Initialize(commandBuffer, commandBufferSize);
    m_CommandListBegin = nullptr;
    m_CommandListEnd = nullptr;
    m_CommandTag = 0;

    if (!m_IsInitializedSendMessageQueue) {
        os::InitializeMessageQueue(&m_SendCommandQueue, m_SendCommandQueueBuffer,
                                   SendCommandQueueCount);
        m_IsInitializedSendMessageQueue = true;
    }

    if (!m_IsInitializedRecvMessageQueue) {
        os::InitializeMessageQueue(&m_RecvCommandQueue, m_RecvCommandQueueBuffer,
                                   RecvCommandQueueCount);
        m_IsInitializedRecvMessageQueue = true;
    }

    m_CommandListCount = 0;
    m_Available = true;
}

// void* CommandManager::AllocMemory(size_t size, bool forceProcessCommandFlag) {}

// bool CommandManager::TryAllocMemory(size_t size) {}

void CommandManager::RecvCommandReply() {
    uintptr_t msg;
    while (os::TryReceiveMessageQueue(&msg, &m_RecvCommandQueue)) {
        Command* commandList{reinterpret_cast<Command*>(msg)};
        FinalizeCommandList(commandList);
    }
}

bool CommandManager::ProcessCommand() {
    uintptr_t msg;
    bool result{os::TryReceiveMessageQueue(&msg, &m_SendCommandQueue)};
    if (!result)
        return false;

    --m_CommandListCount;

    Command* commandList{reinterpret_cast<Command*>(msg)};
    if (commandList->id != InvalidCommand)
        m_pProcessCommandListFunc(commandList);

    os::SendMessageQueue(&m_RecvCommandQueue, msg);
#if NN_WARE_VER < NN_MAKE_VER(3, 5, 1)
    RecvCommandReply();
#endif
    return true;
}

u32 CommandManager::PushCommand(Command* command) {
    if (m_CommandListEnd != nullptr)
        m_CommandListEnd->next = command;
    else
        m_CommandListBegin = command;

    m_CommandListEnd = command;
    command->next = nullptr;
    return m_CommandTag;
}

// u32 CommandManager::FlushCommand(bool forceFlag, bool forceProcessCommandFlag) {}

void CommandManager::FinalizeCommandList(Command* commandList) {
    Command* command{commandList};

    m_FinishCommandTag = command->tag;
    while (commandList != nullptr) {
        command = commandList;
        --m_AllocatedCommandCount;
        commandList = command->next;
    }
    m_CommandBuffer.FreeMemory(command);
}

bool CommandManager::IsFinishCommand(u32 tag) const {
    const u32 limit{(tag - m_FinishCommandTag) / 0x40000000};

    if (tag > m_FinishCommandTag)
        return limit != 0;

    return (m_FinishCommandTag - tag) / 0x40000000 == 0;
}

Command* CommandManager::RecvCommandReplySync() {
    uintptr_t msg;
    bool result{os::TryReceiveMessageQueue(&msg, &m_RecvCommandQueue)};

    if (!result) {
        if (m_pRequestProcessCommandFunc != nullptr)
            m_pRequestProcessCommandFunc();
        os::ReceiveMessageQueue(&msg, &m_RecvCommandQueue);
    }

    Command* commandList{reinterpret_cast<Command*>(msg)};
    FinalizeCommandList(commandList);
    return commandList;
}

void CommandManager::WaitCommandReply([[maybe_unused]] u32 tag) {
#if NN_WARE_VER < NN_MAKE_VER(3, 5, 1)
    while (ProcessCommand()) {
    };
#else
    while (RecvCommandReplySync()->tag != tag) {
    };
#endif
}

size_t CommandManager::GetCommandBufferSize() const {
    return m_CommandBuffer.GetCommandBufferSize();
}

// NON_MATCHING: requires CommandBuffer::GetAllocatableCommandSize
size_t CommandManager::GetAllocatableCommandSize() const {
    return m_CommandBuffer.GetAllocatableCommandSize();
}

// NON_MATCHING: requires CommandBuffer::GetAllocatedCommandBufferSize
size_t CommandManager::GetAllocatedCommandBufferSize() const {
    return m_CommandBuffer.GetAllocatedCommandBufferSize();
}

int CommandManager::GetAllocatedCommandCount() const {
    return m_AllocatedCommandCount;
}

}  // namespace nn::atk::detail
