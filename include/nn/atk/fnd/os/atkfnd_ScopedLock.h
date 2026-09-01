#pragma once

#include <nn/util.h>

namespace nn::atk::detail::fnd {
template <typename TLockObject>
class ScopedLock {
    NN_NO_COPY(ScopedLock);

public:
    explicit ScopedLock(TLockObject& lockObj) 
        : m_LockObj(lockObj) 
    {
        m_LockObj.Lock();
    }

    ~ScopedLock() 
    {
        m_LockObj.Unlock();
    }

private:
    TLockObject& m_LockObj;
};
} // namespace nn::atk::detail::fnd