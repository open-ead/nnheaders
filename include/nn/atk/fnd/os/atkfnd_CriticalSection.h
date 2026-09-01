#pragma once

#include <nn/os/os_Mutex.h>

namespace nn::atk::detail::fnd {
class CriticalSection {
public:
    CriticalSection()
        : m_Mutex(true) {}

    ~CriticalSection();

    void Enter() {
        // TODO
    }

    bool TryEnter() {
        // TODO
        return true;
    }

    void Leave() {
        // TODO
    }

    void Lock() {
        m_Mutex.Lock();
    }

    bool TryLock() {
        return m_Mutex.TryLock();
    }
    
    void Unlock() {
        m_Mutex.Unlock();
    }
    
private:
    os::Mutex m_Mutex;
};
static_assert(sizeof(CriticalSection) == 0x20);
} // namespace nn::atk::detail::fnd