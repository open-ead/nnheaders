#pragma once

#include <nn/os/detail/os_InternalConditionVariable.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/util/util_TypedStorage.h>

namespace nn::os {

namespace detail {
class MultiWaitObjectList;
}  // namespace detail

struct EventType {
    util::TypedStorage<detail::MultiWaitObjectList, 16, 8> _multiWaitObjectList;
    bool _signalState;
    bool _initiallySignaled;
    uint8_t _clearMode;
    uint8_t _state;
    uint32_t _broadcastCounterLower;
    uint32_t _broadcastCounterUpper;
    detail::InternalCriticalSectionStorage _csEvent;
    detail::InternalConditionVariableStorage _cvSignaled;
};
static_assert(std::is_trivial<EventType>::value, "EventType non trivial");

}  // namespace nn::os