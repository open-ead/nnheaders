#pragma once

#include <nn/os/os_Mutex.h>

namespace nn::atk::detail::fnd {
class CriticalSection {
public:
    void lock();
    bool tryLock();
    void unlock();
    
private:
    nn::os::Mutex m_Mutex;
};
static_assert(sizeof(CriticalSection) == 0x20);
} // namespace nn::atk::detail::fnd