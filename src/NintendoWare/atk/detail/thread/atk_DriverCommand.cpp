#include <nn/atk/atk_DriverCommand.h>

namespace nn::atk::detail {

// void DriverCommand::ProcessCommandList(Command *commandList) {}

DriverCommand& DriverCommand::GetInstance() {
    static DriverCommand instance;
    return instance;
}

DriverCommand& DriverCommand::GetInstanceForTaskThread() {
    static DriverCommand instance;
    return instance;
}

DriverCommand::DriverCommand() = default;

void DriverCommand::Initialize(void* commandBuffer, size_t commandBufferSize) {
    CommandManager::Initialize(commandBuffer, commandBufferSize, ProcessCommandList);
    SetRequestProcessCommandFunc(RequestProcessCommand);
}

void DriverCommand::RequestProcessCommand() {
    driver::SoundThread::GetInstance().ForceWakeup();
}

}  // namespace nn::atk::detail
