#include <nn/atk/atk_Task.h>

namespace nn::atk::detail {
Task::Task() 
    : m_Event(os::EventClearMode_ManualClear) {
    
    InitializeStatus();
    SetId(0);

    m_Event.Signal();
}

Task::~Task() = default;
} // namespace nn::atk::detail